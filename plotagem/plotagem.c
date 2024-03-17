#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct TreeNode {
  int data;
  struct TreeNode *left;
  struct TreeNode *right;
} TreeNode;

typedef struct ListNode {
  int data;
  struct ListNode *next;
} ListNode;

int treeIterations = 0;
int listIterations = 0;

TreeNode *createTreeNode(int value) {
  TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
  if (newNode == NULL) {
    printf("Erro ao alocar memória para o novo nó da árvore.\n");
    exit(1);
  }
  newNode->data = value;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

TreeNode *insertTreeNode(TreeNode *root, int value) {
  if (root == NULL) {
    return createTreeNode(value);
  }

  if (value < root->data) {
    root->left = insertTreeNode(root->left, value);
  } else if (value > root->data) {
    root->right = insertTreeNode(root->right, value);
  }

  return root;
}

void inorderTraversal(TreeNode *root) {
  if (root != NULL) {
    inorderTraversal(root->left);
    inorderTraversal(root->right);
  }
}

TreeNode *searchTreeNode(TreeNode *root, int value) {
  if (root == NULL) {
    return NULL;
  }

  if (root->data == value) {
    return root;
  }

  if (value < root->data) {
    treeIterations++;
    return searchTreeNode(root->left, value);
  } else {
    treeIterations++;
    return searchTreeNode(root->right, value);
  }
}

ListNode *createListNode(int value) {
  ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
  if (newNode == NULL) {
    printf("Erro ao alocar memória para o novo nó da lista.\n");
    exit(1);
  }
  newNode->data = value;
  newNode->next = NULL;
  return newNode;
}

void insertListNode(ListNode **headRef, int value) {
  ListNode *newNode = createListNode(value);
  if (*headRef == NULL) {
    *headRef = newNode;
  } else {
    ListNode *current = *headRef;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = newNode;
  }
}

void printList(ListNode *head) {
  ListNode *current = head;
  while (current != NULL) {
    current = current->next;
  }
}

void searchListNode(ListNode *head, int value) {
  ListNode *current = head;
  while (current != NULL) {
    listIterations++;
    if (current->data == value) {
      return;
    }
    current = current->next;
  }

  return;
}

void sortearNumerosUnicos(int quantidade, int inicio, int fim, int *numeros) {
  int i, j, numero_aleatorio, repetido;

  srand((unsigned)time(NULL));

  for (i = 0; i < quantidade; i++) {
    do {
      numero_aleatorio = rand() % (fim - inicio + 1) + inicio;
      repetido = 0;

      for (j = 0; j < i; j++) {
        if (numeros[j] == numero_aleatorio) {
          repetido = 1;
          break;
        }
      }
    } while (repetido);

    numeros[i] = numero_aleatorio;
  }
}

int main() {
  FILE *arquivo1, *arquivo2, *arquivo3;
  int numeros_sorteados[10000];
  int numeros_selecionados[1000];

  arquivo1 = fopen("interacoes_linked.txt", "w");
  arquivo2 = fopen("interacoes_tree.txt", "w");
  arquivo3 = fopen("numero_sorteado.txt", "w");

  if (arquivo1 == NULL) {
    printf("Erro");
    return -1;
  }

  TreeNode *root = NULL;
  ListNode *head = NULL;

  sortearNumerosUnicos(10000, 1, 10000, numeros_sorteados);

  for (int i = 0; i < 10000; i++) {
    root = insertTreeNode(root, numeros_sorteados[i]);
    insertListNode(&head, numeros_sorteados[i]);

    fprintf(arquivo3, "%d\n", numeros_sorteados[i]);
  }

  sortearNumerosUnicos(1000, 1, 10000, numeros_selecionados);

  for (int i = 0; i < 1000; i++) {
    printf("Numero a ser buscado: %d\n", numeros_selecionados[i]);

    listIterations = 0;
    searchListNode(head, numeros_selecionados[i]);
    printf("Numero de interacoes na lista encadeada: %d\n", listIterations);
    fprintf(arquivo1, "%d\n", listIterations);

    treeIterations = 0;
    searchTreeNode(root, numeros_selecionados[i]);
    printf("Numero de interacoes na arvore de busca binaria: %d\n\n", treeIterations);
    fprintf(arquivo2, "%d\n", treeIterations);
  }

  fclose(arquivo1);
  fclose(arquivo2);
  fclose(arquivo3);

  free(root);
  free(head);

  return 0;
}
