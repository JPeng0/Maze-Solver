#define _DEFAULT_SOURCE
#define NUM_ARGS 2
#define MAX_LEN 25000000

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

typedef struct portal
{
    int p;
    int x;
    int y;
} portal;

typedef struct portalPair
{
    portal first;
    portal second;
} pair;

typedef struct coord
{
    int x;
    int y;
    char *path;
    struct coord *next;
} coord;

typedef struct QueueList
{
    int sizeOfQueue;
    size_t memSize;
    coord *head;
    coord *tail;
} Queue;

void queueInit(Queue *q, size_t memSize)
{
    q->sizeOfQueue = 0;
    q->memSize = memSize;
    q->head = q->tail = NULL;
}

bool enqueue(Queue *q, coord *data)
{
    if (data == NULL)
    {
        return false;
    }

    if (q->sizeOfQueue == 0)
    {
        q->head = q->tail = data;
    }
    else
    {
        q->tail->next = data;
        q->tail = data;
    }

    q->sizeOfQueue++;
    return true;
}

coord *dequeue(Queue *q)
{
    if (q->sizeOfQueue > 0)
    {
        coord *temp = q->head;

        if (q->sizeOfQueue > 1)
        {
            q->head = q->head->next;
        }
        else
        {
            q->head = NULL;
            q->tail = NULL;
        }

        q->sizeOfQueue--;
        return temp;
    }
}

int parse_line(char *input, char *tokens[], char *delim)
{
    int i = 0;
    char *tok = strtok(input, delim);

    while (tok != NULL)
    {
        tokens[i] = tok;
        i++;
        tok = strtok(NULL, delim);
    }
    tokens[i] = NULL;
    return i;
}

coord *create_node(int x, int y, char *path)
{

    coord *temp = (coord *)malloc(sizeof(coord));

    //fill the node
    temp->x = x;
    temp->y = y;
    temp->path = (char *)malloc(strlen(path) * sizeof(char) + 1);
    memcpy(temp->path, path, strlen(path));
    temp->next = NULL;
    return temp;
}

void task1(char **tokens, int numToks)
{
    int numS = 0;
    int numD = 0;
    int n = strlen(tokens[0]);
    for (int i = 0; i < numToks; i++)
    {
        if (strlen(tokens[i]) > 5000 || strlen(tokens[i]) != n)
        {
            printf("NO\n");
            return;
        }

        for (int k = 0; k < strlen(tokens[i]); k++)
        {

            if (tokens[i][k] != '#' &&
                tokens[i][k] != 'S' &&
                tokens[i][k] != 'D' &&
                tokens[i][k] != '.')
            {
                printf("NO\n");
                return;
            }
            if (tokens[i][k] == 'S')
            {
                if (numS > 1)
                {
                    printf("NO\n");
                    return;
                }
                numS++;
            }
            if (tokens[i][k] == 'D')
            {
                if (numD > 1)
                {
                    printf("NO\n");
                    return;
                }
                numD++;
            }
        }
    }
    if (numS == 0 || numD == 0 || tokens[numToks - 2] == "")
    {
        printf("NO\n");
        return;
    }
    printf("YES\n");
}

coord *src(char **tokens, int numToks)
{
    for (int i = 0; i < numToks; i++)
    {
        for (int k = 0; k < strlen(tokens[i]); k++)
        {
            if (tokens[i][k] == 'S' || tokens[i][k] == 'D')
            {
                if (i == 0)
                {
                    if (k == 0)
                    {
                        if (tokens[i][k + 1] == '#' && tokens[i + 1][k] == '#')
                        {
                            printf("NO\n");
                            exit(1);
                        }
                    }
                    if (k == strlen(tokens[i]) - 1)
                    {
                        if (tokens[i][k - 1] == '#' && tokens[i + 1][k] == '#')
                        {
                            printf("NO\n");
                            exit(1);
                        }
                    }
                    if (tokens[i][k + 1] == '#' && tokens[i + 1][k] == '#' && tokens[i][k - 1] == '#')
                    {
                        printf("NO\n");
                        exit(1);
                    }
                }
                if (i == numToks - 1)
                {
                    if (k == 0)
                    {
                        if (tokens[i][k + 1] == '#' && tokens[i - 1][k] == '#')
                        {
                            printf("NO\n");
                            exit(1);
                        }
                    }
                    if (k == strlen(tokens[i]) - 1)
                    {
                        if (tokens[i][k - 1] == '#' && tokens[i - 1][k] == '#')
                        {
                            printf("NO\n");
                            exit(1);
                        }
                    }
                    if (tokens[i][k + 1] == '#' && tokens[i - 1][k] == '#' && tokens[i][k - 1] == '#')
                    {
                        printf("NO\n");
                        exit(1);
                    }
                }
                if (tokens[i][k + 1] == '#' && tokens[i - 1][k] == '#' && tokens[i][k - 1] == '#' && tokens[i + 1][k] == '#')
                {
                    printf("NO\n");
                    exit(1);
                }
                if (tokens[i][k] == 'S')
                {

                    return create_node(i, k, "");
                }
            }
        }
    }
}

char *path(char **tokens, int numToks, Queue *q, bool **v, pair *p)
{
    char *path = (char *)malloc(sizeof(char) * MAX_LEN);
    coord *s = src(tokens, numToks);
    s->path = (char *)malloc(10 * sizeof(char));
    enqueue(q, s);
    v[s->x][s->y] = true;
    while (q->sizeOfQueue > 0)
    {
        coord *t = dequeue(q);
        // Destination found;
        if (tokens[t->x][t->y] == 'D')
        {
            return t->path;
        }
        if (isdigit(tokens[t->x][t->y]))
        {

            int val = tokens[t->x][t->y] - '0';
            if (v[p[val].second.x][p[val].second.y] == false)
            {
                coord *new = create_node(p[val].second.x, p[val].second.y, t->path);
                enqueue(q, new);
                v[p[val].second.x][p[val].second.y] = true;
                continue;
            }
            else if (v[p[val].first.x][p[val].first.y] == false)
            {
                coord *new = create_node(p[val].first.x, p[val].first.y, t->path);
                enqueue(q, new);
                v[p[val].first.x][p[val].first.y] = true;
                continue;
            }
        }
        // moving up
        if (t->x - 1 >= 0 &&
            v[t->x - 1][t->y] == false)
        {
            char newPath[10000];
            sprintf(newPath, "%sU", t->path);
            coord *new = create_node(t->x - 1, t->y, newPath);
            enqueue(q, new);
            v[t->x - 1][t->y] = true;
        }

        // moving down
        if (t->x + 1 < numToks &&
            v[t->x + 1][t->y] == false)
        {
            char newPath[10000];
            sprintf(newPath, "%sD", t->path);
            coord *new = create_node(t->x + 1, t->y, newPath);
            enqueue(q, new);
            v[t->x + 1][t->y] = true;
        }

        // moving left
        if (t->y - 1 >= 0 &&
            v[t->x][t->y - 1] == false)
        {
            char newPath[10000];
            sprintf(newPath, "%sL", t->path);
            coord *new = create_node(t->x, t->y - 1, newPath);
            enqueue(q, new);
            v[t->x][t->y - 1] = true;
        }

        // moving right
        if (t->y + 1 < strlen(tokens[0]) &&
            v[t->x][t->y + 1] == false)
        {
            char newPath[10000];
            sprintf(newPath, "%sR", t->path);
            coord *new = create_node(t->x, t->y + 1, newPath);
            enqueue(q, new);
            v[t->x][t->y + 1] = true;
        }
    }
    printf("NO\n");
    exit(1);
}

void task2(char **tokens, int numToks, Queue *q, bool **v, pair *p)
{
    if (strlen(path(tokens, numToks, q, v, p)) > 0)
    {
        printf("YES\n");
        exit(1);
    }
}

void task3(char **tokens, int numToks, Queue *q, bool **v, pair *p)
{
    printf("%s\n", path(tokens, numToks, q, v, p));
}

void task4(char **tokens, int numToks, Queue *q, bool **v, pair *p)
{
    printf("%s\n", path(tokens, numToks, q, v, p));
}

int main(int argc, char *argv[])
{
    char *maze = (char *)malloc(sizeof(char) * MAX_LEN);
    char *line = (char *)malloc(sizeof(char) * 5001);
    ssize_t len = 0;

    if (argc < 2)
    {

        printf("NO\n");
        exit(1);
    }
    int input = atoi(argv[1]);
    if (input > 4 || input < 1)
    {
        printf("NO\n");
        exit(1);
    }

    while (fgets(line, 2, stdin) != NULL)
    {
        strcat(maze, line);
    }

    if (maze[strlen(maze) - 1] != '\n')
    {
        printf("NO\n");
        exit(1);
    }

    for (int i = 0; i < strlen(maze) - 1; i++)
    {
        if (maze[i] == '\n')
        {
            if (maze[i + 1] == '\n')
            {
                printf("NO\n");
                exit(1);
            }
        }
    }

    int numToks;
    char *tokens[5000];
    char *delimit = "\n";

    if ((numToks = parse_line(maze, tokens, delimit)) == -1 || numToks > 5000)
    {
        printf("NO\n");
        exit(1);
    }

    bool *visited[numToks];
    int l = strlen(tokens[0]);
    if (l > 5000)
    {
        printf("NO\n");
        exit(1);
    }
    for (int i = 0; i < numToks; i++)
    {
        visited[i] = (bool *)malloc(l * sizeof(bool));
    }

    pair *portals = (pair *)calloc(10, sizeof(pair));

    for (int i = 0; i < 10; i++){
        portals[i].first.p = -1;
    }

    for (int i = 0; i < numToks; i++)
    {
        if (strlen(tokens[i]) != l)
        {
            printf("NO\n");
            exit(1);
        }
        for (int j = 0; j < l; j++)
        {
            if (tokens[i][j] == '#')
            {
                visited[i][j] = true;
            }
            else
                visited[i][j] = false;
            if (isdigit(tokens[i][j]))
            {
                int val = tokens[i][j] - '0';
                if (portals[val].first.p == -1)
                {
                    portal p = {val, i, j};
                    pair pr = {p, NULL};
                    portals[val] = pr;
                }
                else
                {
                    portal p = {val, i, j};
                    pair pr = {portals[val].first, p};
                    portals[val] = pr;
                }
            }
        }
    }

    
    Queue q;
    queueInit(&q, sizeof(coord));

    switch (input)
    {
    case 1:
        task1(tokens, numToks);
        break;
    case 2:
        task2(tokens, numToks, &q, visited, portals);
        break;
    case 3:
        task3(tokens, numToks, &q, visited, portals);
        break;
    case 4:
        task4(tokens, numToks, &q, visited, portals);
        break;
    }
}