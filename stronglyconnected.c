#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_SIZE 100

int stack[MAX_SIZE], top;

struct adj_list_node {
    int dest;
    struct adj_list_node *next;
};

struct adj_list {
    struct adj_list_node *head;
};

struct Graph {
    int V;
    struct adj_list *array;
};

struct adj_list_node *new_adj_list_node(int dest) {
    struct adj_list_node *newNode = (struct adj_list_node *)malloc(sizeof(struct adj_list_node));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

struct Graph *create_graph(int V) {
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->array = (struct adj_list *)malloc(V * sizeof(struct adj_list));

    for (int i = 0; i < V; ++i) {
        graph->array[i].head = NULL;
    }
    return graph;
}

void get_transpose(struct Graph *gr, int src, int dest) {
    struct adj_list_node *newNode = new_adj_list_node(src);
    newNode->next = gr->array[dest].head;
    gr->array[dest].head = newNode;
}

void add_edge(struct Graph *graph, struct Graph *gr, int src, int dest) {
    struct adj_list_node *newNode = new_adj_list_node(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    get_transpose(gr, src, dest);
}

void print_graph(struct Graph *graph1) {
    for (int v = 0; v < graph1->V; ++v) {
        struct adj_list_node *temp = graph1->array[v].head;
        while (temp) {
            printf("(%d -> %d)\t", v, temp->dest);
            temp = temp->next;
        }
    }
}

void push(int x) {
    if (top >= MAX_SIZE - 1) {
        printf("\n\tSTACK is overflow\n");
    } else {
        stack[++top] = x;
    }
}

int pop() {
    if (top <= -1) {
        printf("\n\tSTACK is underflow\n");
        return -1;
    } else {
        return stack[top--];
    }
}

void set_fill_order(struct Graph *graph, int v, bool visited[], int *stack) {
    visited[v] = true;
    struct adj_list_node *temp = graph->array[v].head;
    while (temp) {
        if (!visited[temp->dest]) {
            set_fill_order(graph, temp->dest, visited, stack);
        }
        temp = temp->next;
    }
    push(v);
}

void dfs_recursive(struct Graph *gr, int v, bool visited[]) {
    visited[v] = true;
    printf("%d ", v);
    struct adj_list_node *temp = gr->array[v].head;
    while (temp) {
        if (!visited[temp->dest]) {
            dfs_recursive(gr, temp->dest, visited);
        }
        temp = temp->next;
    }
}

void strongly_connected_components(struct Graph *graph, struct Graph *gr, int V) {
    bool visited[V];
    for (int i = 0; i < V; i++) {
        visited[i] = false;
    }

    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            set_fill_order(graph, i, visited, stack);
        }
    }

    for (int i = 0; i < V; i++) {
        visited[i] = false;
    }

    int count = 1;
    while (top != -1) {
        int v = pop();
        if (!visited[v]) {
            printf("Strongly Connected Component %d:\n", count++);
            dfs_recursive(gr, v, visited);
            printf("\n");
        }
    }
}

int main() {
    int v, max_edges, i, origin, destin;
    top = -1;
    printf("\nEnter the number of vertices: ");
    scanf("%d", &v);

    struct Graph *graph = create_graph(v);
    struct Graph *gr = create_graph(v);

    max_edges = v * (v - 1);
    for (i = 1; i <= max_edges; i++) {
        printf("Enter edge %d (0 0 to quit): ", i);
        scanf("%d %d", &origin, &destin);

        if ((origin == 0) && (destin == 0)) {
            break;
        }
        if (origin >= v || destin >= v || origin < 0 || destin < 0) {
            printf("Invalid edge!\n");
            i--;
        } else {
            add_edge(graph, gr, origin, destin);
        }
    }

    strongly_connected_components(graph, gr, v);
    return 0;
}
