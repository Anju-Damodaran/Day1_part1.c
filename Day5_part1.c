
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

#define BUFFER_SIZE 128
#define MAX_VERTICES 128
#define EQUAL 1

typedef struct Edge Edge;
struct Vertex
{
    unsigned int color;
    Edge* firstEdge;
};
typedef struct Vertex Vertex;
struct Edge
{
    unsigned int source;
    unsigned int target;
    Edge* nextEdge;
};
struct Graph
{
    unsigned int edgeCount;
    Vertex vertices[MAX_VERTICES];
    Edge edges[MAX_VERTICES * (MAX_VERTICES - 1)];
};
typedef struct Graph Graph;

static void graph(Graph* graph)
{
    unsigned int i;
    graph->edgeCount = 0;
    for (i = 0; i < MAX_VERTICES; i++)
    {
        graph->vertices[i].color = 0;
        graph->vertices[i].firstEdge = NULL;
    }
}

static void graph_add_edge(
    Graph* instance,
    unsigned int source,
    unsigned int target)
{
    Edge* edge = instance->edges + instance->edgeCount;
    edge->source = source;
    edge->target = target;
    edge->nextEdge = instance->vertices[source].firstEdge;
    instance->vertices[source].firstEdge = edge;
    instance->edgeCount++;
    printf("Added edge: %u -> %u\n", source, target);
}

static int sequence_equals(
    const unsigned int left[],
    unsigned int leftLength,
    const unsigned int right[],
    unsigned int rightLength)
{
    unsigned int i;
    if (leftLength != rightLength)
    {
        return 0;
    }
    for (i = 0; i < leftLength; i++)
    {
        if (left[i] != right[i])
        {
            return 0;
        }
    }
    return 1;
}

static unsigned int topological_sort_component(
    unsigned int results[MAX_VERTICES],
    Graph* g,
    unsigned int u)
{
    unsigned int count;
    Edge* e;
    count = 0;
    g->vertices[u].color = 1;
    for (e = g->vertices[u].firstEdge; e; e = e->nextEdge)
    {
        if (!g->vertices[e->target].color)
        {
            count += topological_sort_component(results + count, g, e->target);
        }
    }
    g->vertices[u].color = 2;
    results[count] = u;
    count++;
    return count;
}

static unsigned int topological_sort(
    unsigned int results[MAX_VERTICES],
    const unsigned int vertices[MAX_VERTICES],
    unsigned int vertexCount,
    Graph* g)
{
    unsigned int i, count, u;
    count = 0;
    for (i = 0; i < vertexCount; i++)
    {
        u = vertices[i];
        if (!g->vertices[u].color)
        {
            count += topological_sort_component(results + count, g, u);
        }
    }
    return count;
}

int main()
{
    char cwd[256];
    char buffer[BUFFER_SIZE];
    unsigned int i, u, v, vertexCount, tLength;
    unsigned int vertices[MAX_VERTICES];
    unsigned int t[MAX_VERTICES];
    int subset[MAX_VERTICES];
    char* p;
    Graph parent;
    unsigned int n;
    FILE* file;
    int line_number;

    // Print current working directory for debugging
    getcwd(cwd, sizeof(cwd));
    printf("Current working directory: %s\n", cwd);

    // Open the input file
    file = fopen("C:\\Users\\Dell\\OneDrive\\Documents\\c program assignment\\day 5\\input5.txt", "r");
    if (!file)
    {
        perror("Error opening file");
        return 1;
    }

    // Check if file is empty
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0)
    {
        printf("Input file is empty\n");
        fclose(file);
        return 1;
    }
    fseek(file, 0, SEEK_SET);

    graph(&parent);
    line_number = 0;
    while (fgets(buffer, BUFFER_SIZE, file))
    {
        line_number++;
        buffer[strcspn(buffer, "\n")] = '\0';
        if (*buffer == '\0')
        {
            break; // Empty line separates rules from updates
        }
        if (sscanf(buffer, "%u|%u", &v, &u) != 2)
        {
            printf("Invalid rule format at line %d: %s\n", line_number, buffer);
            continue;
        }
        graph_add_edge(&parent, u, v);
    }

    n = 0;
    while (fgets(buffer, BUFFER_SIZE, file))
    {
        line_number++;
        vertexCount = 0;
        for (i = 0; i < MAX_VERTICES; i++)
        {
            subset[i] = 0;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        if (*buffer == '\0')
        {
            printf("Empty update at line %d\n", line_number);
            continue;
        }
        p = strtok(buffer, ",");
        while (p)
        {
            while (*p == ' ' || *p == '\t') p++; // Trim whitespace
            if (*p == '\0')
            {
                p = strtok(NULL, ",");
                continue;
            }
            u = atoi(p);
            if (u == 0 && strcmp(p, "0") != 0)
            {
                printf("Non-integer in update at line %d: %s\n", line_number, p);
                p = strtok(NULL, ",");
                continue;
            }
            if (u >= MAX_VERTICES)
            {
                printf("Vertex %u exceeds MAX_VERTICES at line %d\n", u, line_number);
                p = strtok(NULL, ",");
                continue;
            }
            subset[u] = 1;
            vertices[vertexCount] = u;
            vertexCount++;
            p = strtok(NULL, ",");
        }
        if (vertexCount == 0)
        {
            printf("Empty update at line %d\n", line_number);
            continue;
        }
        Graph subgraph;
        graph(&subgraph);
        for (i = 0; i < parent.edgeCount; i++)
        {
            Edge* e = parent.edges + i;
            if (subset[e->source] && subset[e->target])
            {
                graph_add_edge(&subgraph, e->source, e->target);
            }
        }
        printf("Update %u: ", n / 1000 + 1);
        for (i = 0; i < vertexCount; i++)
        {
            printf("%u", vertices[i]);
            if (i < vertexCount - 1) printf(",");
        }
        printf(" (size %u)\n", vertexCount);
        tLength = topological_sort(t, vertices, vertexCount, &subgraph);
        printf("Topological sort: ");
        for (i = 0; i < tLength; i++)
        {
            printf("%u", t[i]);
            if (i < tLength - 1) printf(",");
        }
        printf(" (length %u)\n", tLength);
        if (sequence_equals(vertices, vertexCount, t, tLength) == EQUAL)
        {
            n += vertices[vertexCount / 2];
            printf("Valid update %u: middle page = %u\n", n / 1000, vertices[vertexCount / 2]);
        }
        else
        {
            printf("Invalid update %u\n", n / 1000 + 1);
        }
    }
    fclose(file);

    printf("Sum of middle pages: %u\n", n);

    return 0;
}
