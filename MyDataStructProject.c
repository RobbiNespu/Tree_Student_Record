/* Team :
 *           - ROBBINESPU
 *           - BALQIS
 *           - SHIQEN
 * Task :
 *          - Write a program to implements trees in keeping 15 records of class members
 *            contains name and metric number. You are required to print in ascending order.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define KEY_LEN 20

/* Define student data */
struct student
{
    char name[KEY_LEN];                     // Student name
    char matrix[KEY_LEN];                   // Student matrik
    struct student *llink;                  // Left
    struct student *rlink;                  // Right
};

void load_f(void);                          // Load data from file
void save_f(void);                          // Save data to file
void insert_f(void);                        // Insert data input to file
void delete_f(void);                        // Delete data from file
void modify_f(void);                        // Modify data from file
void show_f(void);                          // Show all data via traverse way
void access(char[], char[]);                // The new data is added to a binary search tree
void removing(char[]);                      // Remove data node
void remove_node(struct student *);         // Deleted node and to find alternative node
void inorder(struct student *);             // Traverse by inorder
void postorder(struct student *);           // Traverse by postorder
void preorder(struct student *, FILE *);    // Traverse by postorder
struct student *search(char[]);             // Search node
struct student *search_p(struct student *); // Search parent node

struct student *root, *ptr;

int main()
{
    char option;
    load_f();   /* Mount the file */
    while (1)
    {
        puts("");
        puts("********************");
        puts("     <1> insert");
        puts("     <2> delete");
        puts("     <3> modify");
        puts("     <4> show");
        puts("     <5> quit");
        puts("********************");
        printf("Enter your choice: ");
        option = getche();
        printf("\n\n");
        switch (option)
        {
        case '1':
            insert_f();
            break;
        case '2':
            delete_f();
            break;
        case '3':
            modify_f();
            break;
        case '4':
            show_f();
            break;
        case '5':
            save_f();   /* Save all task of data to file */
            exit(0);
        default:
            puts("Wrong option!");
        }
    }
    return 0;
}

/* Function : Mount file dfile.dat into program */
void load_f(void)
{
    FILE *fptr;
    char name[KEY_LEN];
    char matrix[KEY_LEN];
    printf("File loading...");
    if ((fptr = fopen("dfile.dat", "r")) == NULL)   /* Open file */
    {
        puts("failed!");
        puts("Dfile.dat not found!");
        return;
    }
    while (fscanf(fptr, "%s %s", name, matrix) != EOF) if (strcmp(name, "") != 0) access(name, matrix);
    puts("OK!");
    fclose(fptr);   /* close file */
}

/* Function : Save the file, save it as binary search tree to dfile.dat */
void save_f(void)
{
    FILE *fptr;
    printf("File saving...");
    if ((fptr = fopen("dfile.dat", "w")) == NULL) /* Open file */
    {
        puts("failed!");
        return;
    }
    preorder(root, fptr);     /* The previous order method to write */
    puts("OK!");
    fclose(fptr);             /* Close file */
}

/* Function : Insert new data */
void insert_f(void)
{
    char name[KEY_LEN], matrix[KEY_LEN];
    puts("=====INSERT DATA=====");
    printf("Enter student name: ");
    gets(name);
    printf("Enter student matrix: ");
    gets(matrix);
    access(name, matrix);
}

/* Function : Removes data from the binary search tree */
void delete_f(void)
{
    char name[20];
    if (root == NULL)
    {
        puts("No student record!");
        return;
    }
    puts("=====DELETE DATA=====");
    printf("Enter student name: ");
    gets(name);
    removing(name);
}



/* Function : the data is output to the screen */
void show_f(void)
{
    if (root == NULL)
    {         /* Determine whether the root node is NULL*/
        puts("No student record!");
        return;
    }
    puts("=====SHOW DATA=====");
    inorder(root);              /* Output data inorder */
    printf("\nPostorder: \n");
    postorder(root);            /* Output data postorder */
    putchar('\n');
}

/* Function : the new data is added to a binary search tree */
void access(char name[], char matrix[])
{
    struct student *node, *prev;
    if (search(name) != NULL)
    { /* Data already exists then display an error */
        printf("Student %s has existed!\n", name);
        return;
    }
    ptr = (struct student *) malloc(sizeof(struct student));
    strcpy(ptr->name, name);
    strcpy(ptr->matrix, matrix);
    ptr->llink = ptr->rlink = NULL;
    if (root == NULL)           /* If the root node is NULL status */
            root = ptr;
    else
    {                      /* When the root node cannot be NULL */
            node = root;
            while (node != NULL)
            {  /* Search the data insertion point */
                prev = node;
            if (strcmp(ptr->name, node->name) < 0)
                node = node->llink;
            else
                node = node->rlink;
            }
        if (strcmp(ptr->name, prev->name) < 0)
            prev->llink = ptr;
        else
            prev->rlink = ptr;
    }
}

/* Function : Modify data */
void modify_f(void)
{
    struct student *node;
    char name[KEY_LEN], temp[KEY_LEN];
    if (root == NULL)
    {         /* Determine whether the root node is NULL*/
        puts("No student record!");
        return;
    }
    puts("=====MODIFY DATA===== ");
    printf("Enter student name: ");
    gets(name);
    if ((node = search(name)) == NULL)
            printf("Student %s not found!\n", name);
    else
    {
            /* List the original data */
            printf("Original student name: %s\n", node->name);
            printf("Original student matrix: %s\n", node->matrix);
            printf("Enter new matrix: ");
            gets(temp);
            strcpy(node->matrix,temp);
            printf("Data of student %s modified\n", name);
    }
}

/* Function : Removes data from a binary search tree */
void removing(char name[])
{
    struct student *del_node;
    if ((del_node = search(name)) == NULL)
    {   /* Data not found error is displayed */
        printf("Student %s not found!\n", name);
    }
    else
    {
        remove_node(del_node);
        printf("Data of student %s deleted!\n", name);
    }
}

/* Function : Remove node and if necessary, to find an alternative node */
void remove_node(struct student *node)
{
    struct student *parent, *candidate;
    char key[KEY_LEN];
    if ((node->llink == NULL) && (node->rlink == NULL))
    {
        /* leaf node */
        if (node == root)
        {
            free(root);
            root = NULL;
        } else
        {
            parent = search_p(node);
            if (parent->llink == node)
            {
                parent->llink = NULL;
            }
            else
            {
                parent->rlink = NULL;
            }
            free(node);
        }
    }
    else if (node->llink != NULL)
    {
       /* Find the left most node of the tree */
        candidate = node->llink;
        while (candidate->rlink != NULL)
        {
            candidate = candidate->rlink;
        }
        strcpy(key, candidate->name);
        strcpy(node->matrix, candidate->matrix);
        remove_node(candidate);
        strcpy(node->name, key);
    }
    else
    {
        /* Find the right subtree minimum node */
        candidate = node->rlink;
        while (candidate->llink != NULL)
        {
            candidate = candidate->llink;
        }
        strcpy(key, candidate->name);
        strcpy(node->matrix, candidate->matrix);
        remove_node(candidate);
        strcpy(node->name, key);
    }
}

/* Function : Traverse inorder via recursion */
void inorder(struct student *node)
{
    if (node != NULL)
    {
        inorder(node->llink);
        printf(" %-15s %-15s\n", node->name, node->matrix);
        inorder(node->rlink);
    }
}

/* Function : Traverse postorder via recursion */
void postorder(struct student *node)
{
     if (node != NULL)
     {
         postorder(node->llink);
         postorder(node->rlink);
         printf(" %s \n", node->name);
     }
}

/* The previous order method to write data to files, mining recursion */
void preorder(struct student *node, FILE * fptr)
{
    if (node != NULL) {
        fprintf(fptr, "%s %s\n", node->name, node->matrix);
        preorder(node->llink, fptr);
        preorder(node->rlink, fptr);
    }
}

/* Function : Search for target node */
struct student *search(char target[])
{
    struct student *node;
    node = root;
    while (node != NULL) {
        if (strcmp(target, node->name) == 0)
            return node;
        else
           /*Target is less than the current node search to the left */
        if (strcmp(target, node->name) < 0)
            node = node->llink;
        else                    /*Target is greater than the current node to the right search */
            node = node->rlink;
    }
    return node;
}

/* Function : Search for the parent node of the node, the root has no parent*/
struct student *search_p(struct student *node)
{
    struct student *parent;
    if (node == root)
        return NULL;
    parent = root;
    while ((parent->llink != NULL) || (parent->rlink != NULL)) {
        if ((parent->llink == node) || (parent->rlink == node)) {
            return parent;
        }
        parent = (strcmp(node->name, parent->name) < 0) ?
            parent->llink : parent->rlink;
        if (parent == NULL)
            return NULL;
    }
    return NULL;
}
