// James Simmons
// Practice with stacks and queues
// Conversion of infix arithmetic notation to postfix arithmetic notation
// Evaluation of postfix arithmetic notation
// 2/28/2019

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Return value for empty stacks and queues
#define EMPTY INT_MIN

// Linked List
// Holds either character or integer information, depending on isNum
typedef struct node
{
	int data;
	int isNum;
	struct node *next;
} node;

// Stack structure implemented via linked lists
typedef struct stack
{
	node *top;
	int size;
} stack;

// Queue structure implemented via linked lists
typedef struct queue
{
	node *front;
	node *back;
	int size;
} queue;

node *makeNode(int data, int isNum);
void printq(queue *q);
queue *infixToPostfix(char *infix);
int postfixToInt(queue *postfix);
queue *enq(queue *q, node *temp);
node *deq(queue *q);
int front(queue *q);
int frontIsNum(queue *q);
int isEmptyq(queue *q);
stack *push(stack *s, node *temp);
node *pop(stack *s);
int peek(stack *s);
int isEmpty(stack *s);
stack *destroyStack(stack *s);
queue *destroyQueue(queue *q);
int power(int a, int b);

// Take in a single command-line argument, a string
// representing an infix-notation arithmetic expression.
// Convert said string into a queue of tokens (numbers
// and symbols) in postfix-notation.
// Use postfix queue to evaluate the expression.
int main(int argc, char **argv)
{
	queue *postfix = NULL;
	int answer = 0;

	if (argc < 2)
		return 0;

	printf("%s\n", argv[1]);

	postfix = infixToPostfix(argv[1]);
	printq(postfix);

	answer = postfixToInt(postfix);
	printf("%d\n", answer);

	postfix = destroyQueue(postfix);
	return 0;
}

// Allocate and return a node containing data
// and whether to interpret said data as an
// integer or a character.
node *makeNode(int data, int isNum)
{
	node *temp;
	if ((temp = malloc(sizeof(node))) == NULL)
		return NULL;
	temp->data = data;
	temp->isNum = isNum;
	temp->next = NULL;
	return temp;
}

// Traverse the linked list to print the contents of the queue,
// taking care to represent integers and characters appropriately.
void printq(queue *q)
{
	node *temp;

	if (isEmptyq(q))
	{
		printf("(empty queue)\n");
		return;
	}

	for (temp = q->front; temp != NULL; temp = temp->next)
	{
		if (temp->isNum)
			printf("%d ", temp->data);
		else
			printf("%c ", temp->data);
	}

	printf("\n");
}

// Given a string representing an infix arithmetic expression,
// allocate and return a queue representing the same expression
// in postfix notation.
queue *infixToPostfix(char *infix)
{
	int sym, i = 0;
	node *temp = NULL;
	queue *postfix = NULL;
	stack *s = NULL;

	if (infix == NULL)
		return NULL;

	// Each iteration of this loop adds one node to the queue
	for (i = 0; (sym = infix[i]) != '\0'; i++)
	{
		// Consecutive characters in the string may represent parts
		// of one number. Advance forward to gather all of these
		// characters before adding the full number to the queue.
		if (isdigit(sym))
		{
			temp = makeNode(sym - '0', 1);

			while (isdigit(infix[i+1]))
			{
				sym = infix[++i];
				temp->data = (temp->data * 10) + (sym - '0');
			}

			postfix = enq(postfix, temp);
			temp = NULL;
		}

		// When we reach a close-parenthesis, move all nodes from
		// the stack to the queue until reaching the corresponding
		// open-parenthesis, which we remove but do not enqueue.
		else if (sym == ')')
		{
			while (peek(s) != '(')
				postfix = enq(postfix, pop(s));
			free(pop(s));
		}

		// Push any arithmetic symbols onto the stack, taking care to pop
		// existing symbols off if required by the rules of postfix notation.
		// Simply ignore all other characters.
		else
		{
			switch (sym)
			{
				case '+':
				case '-':
					while (!(peek(s) == '(' || isEmpty(s)))
						postfix = enq(postfix, pop(s));

				case '*':
				case '/':
				case '%':
					if (peek(s) == '*' || peek(s) == '/' || peek(s) == '%' || peek(s) == '^')
						postfix = enq(postfix, pop(s));

				case '^':
					if (peek(s) == '^')
						postfix = enq(postfix, pop(s));

				case '(':
					s = push(s, makeNode(sym, 0));
			}
		}
	}

	// Once we have concluded moving all numbers into the queue,
	// empty out the stack into the queue.
	while (!isEmpty(s))
		postfix = enq(postfix, pop(s));

	s = destroyStack(s);

	return postfix;
}

// Given a queue representing an arithmetic expression
// in postfix notation, evaluate the expression.
int postfixToInt(queue *postfix)
{
	int answer;
	char sym;
	node *temp, *op1, *op2;
	stack *s = NULL;

	// Each iteration of this loop processes and
	// removes one node in the queue.
	while (!isEmptyq(postfix))
	{
		// Push all numbers onto the stack as we encounter them.
		if (frontIsNum(postfix))
		{
			s = push(s, deq(postfix));
		}

		// When we encounter a symbol: pop two numbers off the stack,
		// evaluate them according to the symbol, and push the result
		// back onto the stack.
		else
		{
			temp = deq(postfix);
			sym = temp->data;
			free(temp);

			op2 = pop(s);
			op1 = pop(s);

			switch (sym)
			{
				case '+':
					answer = op1->data + op2->data;
					break;
				case '-':
					answer = op1->data - op2->data;
					break;
				case '*':
					answer = op1->data * op2->data;
					break;
				case '/':
					answer = op1->data / op2->data;
					break;
				case '%':
					answer = op1->data % op2->data;
					break;
				case '^':
					answer = power(op1->data, op2->data);
					break;
				default:
					answer = 0;
					printf("ERROR! ERROR! BAD SYMBOL IN postfixToInt()!\n");
			}

			s = push(s, makeNode(answer, 1));
			free(op1);
			free(op2);
		}
	}

	// When the evaluation is complete, there remains
	// only one number in the stack: the answer.
	temp = pop(s);
	answer = temp->data;
	free(temp);
	s = destroyStack(s);
	return answer;
}

// Insert the node to the back of the queue.
// (Linked-List Tail-Insertion)
queue *enq(queue *q, node *temp)
{
	if (q == NULL)
		if ((q = calloc(1, sizeof(queue))) == NULL)
			return NULL;

	if (temp == NULL)
		return q;

	if (isEmptyq(q))
	{
		q->front = q->back = temp;
		q->size++;
		return q;
	}

	q->back->next = temp;
	q->back = temp;
	q->size++;

	return q;
}

// Retrieve and return the node at the front of the queue.
// (Linked-List Head-Deletion)
node *deq(queue *q)
{
	node *temp;

	if (isEmptyq(q))
		return NULL;

	if (q->front == q->back)
		q->back == NULL;

	temp = q->front;
	q->front = temp->next;
	temp->next = NULL;
	q->size--;

	return temp;
}

// Return the data within the node at the front of the queue
// without removing it from the queue.
int front(queue *q)
{
	return isEmptyq(q) ? EMPTY : q->front->data;
}

// Return whether the node at the front of the queue is an
// integer or a character.
int frontIsNum(queue *q)
{
	return isEmptyq(q) ? 0 : q->front->isNum;
}

// Return whether the queue contains no nodes.
int isEmptyq(queue *q)
{
	return q == NULL || q->size == 0;
}

// Insert the node on top of the stack
// (Linked-List Head-Insertion)
stack *push(stack *s, node *temp)
{
	if (s == NULL)
		if ((s = calloc(1, sizeof(stack))) == NULL)
			return NULL;

	if (temp == NULL)
		return s;

	temp->next = s->top;
	s->top = temp;
	s->size++;

	return s;
}

// Retrieve and return the node from the top of the stack.
// (Linked-List Head-Deletion)
node *pop(stack *s)
{
	node *temp;

	if (isEmpty(s))
		return NULL;

	temp = s->top;
	s->top = temp->next;
	temp->next = NULL;
	s->size--;

	return temp;
}

// Return the data within the node on the top of the stack
// without removing it from the stack.
int peek(stack *s)
{
	return isEmpty(s) ? EMPTY : s->top->data;
}

// Return whether the stack contains no nodes.
int isEmpty(stack *s)
{
	return s == NULL || s->size == 0;
}

// Deallocate all nodes within the stack,
// and the stack struct itself.
stack *destroyStack(stack *s)
{
	if (s == NULL)
		return NULL;

	while (!isEmpty(s))
		free(pop(s));

	free(s);
	return NULL;
}

// Deallocate all nodes within the queue,
// and the queue struct itself.
queue *destroyQueue(queue *q)
{
	if (q == NULL)
		return NULL;

	while (!isEmptyq(q))
		free(deq(q));

	free(q);
	return NULL;
}


// Evaluates a raised to the power of b
int power(int a, int b)
{
	int root, extra;
	if (b == 0)
		return 1;
	root = power(a, b/2);
	extra = b % 2 ? a : 1;
	return root * root * extra;
}
