/*
Created by Steven Shea on 3/14/15.
for CMPS 1600, project 2
*/

#include <animal/animal_game.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>



slsBTree *sls_animaltree_new()
{
    slsBTree *tree = NULL;
    tree = sls_btree_new(
        sls_animal_copy,
        sls_animal_free);


    return tree;
}

void *sls_animal_copy(void const *data)
{
    slsAnimalData const *t_data = data;
    slsAnimalData *copied = NULL;
    size_t description_size = 1;

    

    if (t_data) {
        copied = sls_animal_new(
            t_data->is_species, 
            t_data->description);
    }


    return copied;
}
void sls_animal_free(void *data)
{
    slsAnimalData *t_data = data;
    if (t_data) {
        if (t_data->description) {
            free(t_data->description);
        }
        free(t_data);
    }
}

slsAnimalData *sls_animal_new(
    slsBool is_species, 
    char const *description)
{
    slsAnimalData *data = NULL;
    data = calloc(sizeof(slsAnimalData), 1);
    if (data) {
        data->is_species = is_species;

        size_t description_size = 1;
        if (description) {
            description_size = strlen(description);
        }
        data->description = sls_stringalloc(description, description_size);
        data->description[description_size] = '\0';
    }

    return data;
}

slsResponse sls_parse_response(char const *res) 
{
    char const *ptr = res;
    const size_t len = strlen(res);
    slsResponse res_value;
    int i;

    /* search for first alphabetical value */
    for (i=0; i<len; ++i) {
        if (!isalpha(*ptr)) {
            ++ptr;
        } else { break; }
    }


    int c = tolower(*ptr);

    /* 
    use a switch statement to match the first letter in response
    to a valid response char (y, n, or q)
    */
    switch (c) {
        case 'y':
            res_value = SLS_YES;
            break;
        case 'n':
            res_value = SLS_NO;
            break;
        case 'q':
            res_value = SLS_QUIT;
            break;
        default:
            res_value = SLS_UNDETERMINED;
    }

    return res_value;
}

slsBNode *sls_animalnode_new(
    slsBTree *tree,
    slsBool is_species, 
    char const *description)
{
    slsAnimalData *data;
    slsBNode *node;
    data = sls_animal_new(is_species, description);
    node = sls_bnode_new(
        tree,
        data,
        NULL,
        NULL);

    sls_animal_free(data);
    return node;
}

slsResponse sls_ask_question(slsBNode *node)
{
    if (!node || !node->val) {
        return SLS_UNDETERMINED;
    }
    slsAnimalData *data = node->val;
    slsResponse res= SLS_UNDETERMINED;
    char const *prompt = data->is_species?
        "Are you thinking of a":
        "Does your animal have";
    
    while (res == SLS_UNDETERMINED) {
        char *line;
        fprintf(stderr, "\n%s %s?\n->", prompt, data->description);
        line = sls_getline(stdin, SLS_MAX_INPUT_SIZE);

        res = sls_parse_response(line);
        if (res == SLS_UNDETERMINED) {
            fprintf(stderr, "Sorry, I could not read your answer\n");
        }

        free(line);
    }
    
    return res;
}

slsBNode *sls_ask_new_category(slsBNode *node)
{
    if (!node || !node->val || !node->tree) {
        assert(SLS_FALSE);
        return NULL;
    }
    slsAnimalData *data = node->val;
    slsBNode *new_node = NULL;

    char *line = NULL;
    fprintf(stderr, 
        "\nWhat does your animal have that a %s doesn't:\n->",
        data->description);

    line = sls_getline(stdin, SLS_MAX_INPUT_SIZE);

    if (!line) {
        assert(SLS_FALSE);
        return NULL;
    }
    new_node = sls_animalnode_new(
        node->tree, 
        SLS_FALSE,
        line);

    free(line);

    return new_node;
}

slsBNode *sls_ask_new_animal(slsBNode *node)
{

    if (!node || !node->val || !node->tree) {
        assert(0);
        return NULL;
    }
    slsAnimalData *data = node->val;
    slsBNode *new_node = NULL;

    char *line;
    fprintf(stderr, 
        "\nwhat is your animal?:\n->");
    line = sls_getline(stdin, SLS_MAX_INPUT_SIZE);
    new_node = sls_animalnode_new(
        node->tree, 
        SLS_TRUE,
        line);
    free(line);

    return new_node;
}

void sls_print_node(slsBNode const *node)
{

}

slsBNode **sls_attempt_traversal(
    slsBNode *node, 
    slsResponse res)
{
    slsBNode **child_ptr = (res == SLS_YES)?
        &(node->right):
        &(node->left);

    return child_ptr;
}

slsBNode *sls_decide_response(
    slsBNode *node, 
    slsResponse res)
{
    if (!node || !node->val || !node->tree) {
        assert(0);
        return NULL;
    }
    slsAnimalData *data = node->val;
    slsBNode *new_node = *sls_attempt_traversal(node, res);
    if (new_node) {
        char const *dirrection = (new_node == node->left)?
            "left": "right";

        fprintf(stderr, "moving to the %s node\n", dirrection);
        return new_node;
    }

    

    if (res == SLS_YES && data->is_species) {
        fprintf(stderr, "I guessed your animal!\nLet's play again\n");
        new_node = node->tree->head;
    } else if (res == SLS_NO && data->is_species) {
        fprintf(stderr, "I guessed wrong.\n");
        new_node = sls_ask_new_category(node);
        slsBNode *parent = node->parent;
        node->parent = new_node;
        new_node->parent = parent;

        if (parent->left == node) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
    } else {
        
    }


    return new_node;
}



