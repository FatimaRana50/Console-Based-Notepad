#include <iostream>
#include <windows.h>
#include <fstream>
#include <filesystem>
#include <iomanip>
using namespace std;
int x = 1, y = 1;
#define VK_Z 0x5A
namespace fs = std::filesystem;
//basic node 
struct Node
{

	char c;
	Node* next;
	Node* prev;
	Node* up;
	Node* down;
	Node(char a)
	{
		c = a;
		next = nullptr;
		prev = nullptr;
		up = nullptr;
		down = nullptr;
	}

};
class Line
{
public:
	Node* head;
	int charcount;
	Line* up;
	Line* down;
	bool overflow;
	Line()
	{
		head = nullptr;
		charcount = 0;
		up = nullptr;
		down = nullptr;
		overflow = false;

	}
	// insert character by character into link list
	void insertion(char ch, int pos) {
		Node* newNode = new Node(ch);

		if (!head) { // If the line is empty
			head = newNode;
			charcount++;
			return;
		}

		Node* current = head;

		for (int i = 1; i < pos && current; i++) {
			current = current->next;
		}

		if (current) {
			newNode->next = current;
			newNode->prev = current->prev;

			if (current->prev) {
				current->prev->next = newNode;
			}
			else {
				head = newNode;
			}
			current->prev = newNode;
		}
		else {
			current = head;
			while (current->next) {
				current = current->next;
			}
			current->next = newNode;
			newNode->prev = current;
		}
		maintainVerticalLinks(newNode);
		charcount++;
		/*if (charcount == 95)
		{
			overflow = true;
		}*/

	}
	//for mainting the links between node properly
	void maintainVerticalLinks(Node* newNode) {

		if (up) {
			newNode->up = up->head;
			if (up->head) {
				Node* aboveNode = up->head;

				for (int i = 0; i < charcount - 1 && aboveNode; i++) {
					aboveNode = aboveNode->next;
				}
				if (aboveNode) {
					aboveNode->down = newNode;
				}
			}
		}
		else {
			newNode->up = nullptr;
		}

		if (down) {
			newNode->down = down->head;
			if (down->head) {
				Node* belowNode = down->head;

				for (int i = 0; i < charcount - 1 && belowNode; i++) {
					belowNode = belowNode->next;
				}
				if (belowNode) {
					belowNode->up = newNode;
				}
			}
		}
		else {
			newNode->down = nullptr; // No line below
		}
	}
	// identifying the end of line
	bool EndOfLine(int col) {

		Node* current = head;
		int count = 0;


		while (current) {
			if (count >= col) {
				return false;
			}
			current = current->next;
			count++;
		}


		return true;
	}
	//splitting the lines
	void splitLine(int col) {

		Line* newLine = new Line();


		Node* currentNode = head;
		for (int i = 0; i < col; i++) {
			currentNode = currentNode->next;
		}


		newLine->head = currentNode;

		if (currentNode != nullptr) {
			currentNode->prev->next = nullptr;
			currentNode->prev = nullptr;
		}

		newLine->up = this;
		newLine->down = this->down;


		if (this->down != nullptr) {
			this->down->up = newLine;
		}


		this->down = newLine;


		Node* nodeInOldLine = head;
		Node* nodeInNewLine = newLine->head;
		Line* nextLine = newLine->down;

		while (nodeInNewLine != nullptr) {

			if (nextLine != nullptr && nextLine->head != nullptr) {
				Node* correspondingNodeInNextLine = nextLine->head;
				int steps = 0;


				while (steps < col && correspondingNodeInNextLine != nullptr) {
					correspondingNodeInNextLine = correspondingNodeInNextLine->next;
					steps++;
				}


				if (correspondingNodeInNextLine != nullptr) {
					nodeInNewLine->down = correspondingNodeInNextLine;
					correspondingNodeInNextLine->up = nodeInNewLine;
				}
			}


			if (nodeInOldLine != nullptr) {
				nodeInOldLine->down = nodeInNewLine;
				nodeInNewLine->up = nodeInOldLine;
			}


			nodeInOldLine = nodeInOldLine != nullptr ? nodeInOldLine->next : nullptr;
			nodeInNewLine = nodeInNewLine->next;
		}
	}
	int getlength()
	{
		Node* temp = head;
		int length = 0;
		while (temp)
		{
			length++;
			temp = temp->next;
		}
		return length;
	}
	void moveToLine(Line* dest, int aakhrispace);

	//brings the required node from the list
	Node* nodelkeao(int index)
	{
		Node* currentNode = head;
		int count = 0;


		while (currentNode != nullptr && count < index) {
			currentNode = currentNode->next;
			count++;
		}


		return currentNode;
	}
	Node* bringnode(int index)
	{
		Node* currentNode = head;
		int count = 0;


		while (currentNode->next != nullptr && count < index) {
			currentNode = currentNode->next;
			count++;
		}


		return currentNode;
	}
	bool isempty()
	{
		return head == nullptr;
	}
	// delete the node from the list
	void nodedeletion(int col) {
		if (!head) {
			return;
		}

		Node* current = head;


		for (int i = 1; i < col && current; i++) {
			current = current->next;
		}

		if (current) {

			if (current->prev) {
				current->prev->next = current->next;
			}
			else {

				head = current->next;
			}

			if (current->next) {
				current->next->prev = current->prev;
			}








			Node* nextNode = current->next;
			while (nextNode) {

				if (nextNode->up) {
					nextNode->up->down = nextNode;
				}
				if (nextNode->down) {
					nextNode->down->up = nextNode;
				}

				nextNode = nextNode->next;
			}
			delete current;
			charcount--;
		}
	}
	//helper function for implementing backspace functionality
	void backspaceHelper(Line* nextLine, Node* wordStart, Node* wordEnd) {

		this->head = wordEnd->next; // If moving the first word, update head



		if (wordEnd->next != nullptr) {
			wordEnd->next->prev = wordStart->prev;
		}


		wordStart->prev = nullptr;
		wordEnd->next = nullptr;


		if (nextLine->getlength() == 0) {
			nextLine->head = wordStart;
		}
		else {
			Node* lastNode = nextLine->head;
			while (lastNode->next != nullptr) {
				lastNode = lastNode->next; // Traverseing to the end
			}

			Node* spaceNode = new Node(' ');
			nextLine->charcount++;
			/*lastNode->next = wordStart;
			wordStart->prev = lastNode;*/
			lastNode->next = spaceNode;
			spaceNode->prev = lastNode;

			spaceNode->next = wordStart;
			wordStart->prev = spaceNode;


			lastNode = wordEnd;
		}
	}
	//deletes the line
	void deleteLine() {
		Node* currentNode = head;
		while (currentNode) {
			Node* tempNode = currentNode;
			currentNode = currentNode->next;
			delete tempNode;
		}
		delete head;
	}



};
class Mylinkedlist
{
public:
	Line* linehead;

	int totallines;
	int currentlines;
	Mylinkedlist()
	{
		linehead = nullptr;

		totallines = 20;
		currentlines = 0;
	}
	//insertion of charctres in the list
	void insert(int row, int col, char ch) {

		if (!linehead) {
			Line* newl = new Line();
			linehead = newl;
			currentlines++;
			linehead->insertion(ch, col);
			return;
		}

		// Traversel to the specified row sent
		Line* current = linehead;
		for (int i = 1; i < row && current->down != nullptr; i++) {
			current = current->down;
		}



		if (ch == '\n') {
			//appendLine(); // Append a new line
			//return;

			if (current->EndOfLine(col)) {
				appendLine();
			}
			else {
				current->splitLine(col);
			}
			return;
		}


		current->insertion(ch, col);
	}

	//displaying the content on console
	void display() {
		if (!linehead) {
			return;
		}

		Line* currentLine = linehead;

		while (currentLine) {
			Node* currentChar = currentLine->head;

			while (currentChar) {
				cout << currentChar->c;
				currentChar = currentChar->next;
			}

			cout << endl;
			cout << "|";
			currentLine = currentLine->down; // mve to the next line
		}
	}
	//appending line after line
	void appendLine() {
		Line* newLine = new Line(); // create a new Line object


		if (!linehead) {
			linehead = newLine;
			currentlines++;
			return;
		}


		Line* current = linehead;
		while (current->down) {
			current = current->down;
		}


		current->down = newLine;
		newLine->up = current;

		newLine->head = nullptr;


		currentlines++;


		if (currentlines > totallines) {

			return;
		}
	}
	// calculates the line length
	int getlinelength(int y)
	{
		Line* currentline = linehead;
		int linecount = 1;
		while (linecount < y && currentline != nullptr)
		{
			currentline = currentline->down;
			linecount++;
		}
		if (currentline == nullptr) {
			return 0;
		}
		Node* temp = currentline->head;
		int len = 0;
		while (temp)
		{
			len++;
			temp = temp->next;
		}
		return len;

	}
	//get the node at given x,y position
	Node* getNodeAt(int y, int x) {
		Line* currentLine = linehead;
		int lineCount = 1;


		while (lineCount < y && currentLine != nullptr) {
			currentLine = currentLine->down;
			lineCount++;
		}


		if (currentLine == nullptr) {
			return nullptr;
		}


		Node* currentNode = currentLine->head;
		int colCount = 1;

		while (currentNode != nullptr && colCount < x) {
			currentNode = currentNode->next;
			colCount++;
		}


		if (colCount != x || currentNode == nullptr) {
			return nullptr;
		}

		return currentNode;
	}
	//get line at given y position
	Line* getLineAt(int y) {
		Line* current = linehead;


		int count = 1;
		while (current != nullptr && count < y) {
			current = current->down;
			count++;
		}


		return current;
	}

	void linekadeletion(int row, int col) {
		if (!linehead) {
			return;  // No lines exist
		}

		Line* current = linehead;


		for (int i = 1; i < row && current->down != nullptr; i++) {
			current = current->down;
		}


		current->nodedeletion(col);


	}
	//deletes the whole line if it is empty
	void linepooridelete(int row)
	{
		Line* current = linehead;


		for (int i = 1; i < row && current->down != nullptr; i++) {
			current = current->down;
		}

		if (current->isempty()) {

			if (current->up && current->down) {
				Node* nodeAbove = current->up->head;
				Node* nodeBelow = current->down->head;


				while (nodeAbove && nodeBelow) {
					nodeAbove->down = nodeBelow;
					nodeBelow->up = nodeAbove;

					nodeAbove = nodeAbove->next;
					nodeBelow = nodeBelow->next;
				}
			}


			if (current->up) {
				current->up->down = current->down;
			}
			else {
				linehead = current->down;
			}

			if (current->down) {
				current->down->up = current->up;
			}


			delete current;
			currentlines--;
		}


	}
	void removeEmptyLine(int lineIndex);
	//if previous line have space the words are moved back tp previous line
	void moveWordsToPreviousLine(int currentLineIndex) {
		if (currentLineIndex <= 0) return;

		Line* currentLine = linehead;
		for (int i = 1; i < currentLineIndex && currentLine; i++) {
			currentLine = currentLine->down;
		}

		if (currentLine && currentLine->up) {
			Line* previousLine = currentLine->up;
			Node* current = currentLine->head;
			Node* prev = nullptr;

			int previousLineLength = previousLine->getlength();
			int spaceAvailable = 95 - previousLineLength;


			while (current && spaceAvailable > 0) {
				Node* next = current->next;

				int wordlen = 0;
				Node* temp = current;
				while (temp)
				{
					if (temp->c != ' ')
					{
						wordlen++;

					}
					temp = temp->next;
				}

				if (spaceAvailable > 0 && wordlen <= spaceAvailable) {

					if (previousLine->getlength() == 0) {
						previousLine->head = current;
					}
					else {
						Node* lastNode = previousLine->head;
						while (lastNode->next) {
							lastNode = lastNode->next;
						}
						lastNode->next = current;
						current->prev = lastNode;
					}


					if (prev) {
						prev->next = nullptr;
					}
					else {
						currentLine->head = next;
					}

					current->next = nullptr;




					Node* nodeAbove = previousLine->head;
					Node* nodeBelow = currentLine->head;

					while (nodeAbove && nodeBelow) {
						nodeAbove->down = nodeBelow;
						nodeBelow->up = nodeAbove;

						nodeAbove = nodeAbove->next;
						nodeBelow = nodeBelow->next;
					}


					while (nodeAbove) {
						nodeAbove->down = nullptr;
						nodeAbove = nodeAbove->next;
					}
					while (nodeBelow) {
						nodeBelow->up = nullptr;
						nodeBelow = nodeBelow->next;
					}

					spaceAvailable--;
					previousLine->charcount++;
					currentLine->charcount--;


					current = next;
				}
			}



			if (currentLine->getlength() == 0) {
				linepooridelete(currentLineIndex);
			}
		}
	}



	// after backspace if there is overflow condition it will be handled in this function
	void backspaceKaOverflow(Line* line) {
		Line* lineAbove = line->up; // Line 1
		if (lineAbove == nullptr) {
			return;
		}


		int totalLength = line->getlength() + lineAbove->getlength();


		while (totalLength > 94) {
			Node* wordStart = line->head;
			Node* wordEnd = wordStart;


			while (wordEnd != nullptr && wordEnd->next != nullptr && wordEnd->next->c != ' ') {
				wordEnd = wordEnd->next;
			}


			int wordLength = 0;
			Node* temp = wordStart;
			while (temp != wordEnd->next) {
				wordLength++;
				temp = temp->next;
			}


			if (lineAbove->getlength() + wordLength <= 94) {

				line->backspaceHelper(lineAbove, wordStart, wordEnd);
				line->charcount -= wordLength;
				line->overflow = true;
			}
			else {

				break;
			}


			totalLength = line->getlength() + lineAbove->getlength();
		}
		while (line->head != nullptr && line->head->c == ' ') {
			Node* temp = line->head;
			line->head = line->head->next;

			if (line->head != nullptr) {
				line->head->prev = nullptr;
			}

			delete temp;
			line->charcount--;
		}

		Node* nodeooper = lineAbove->head;
		Node* nodeneeche = line->head;

		while (nodeooper != nullptr && nodeneeche != nullptr) {

			nodeooper->down = nodeneeche;
			nodeneeche->up = nodeooper;


			nodeooper = nodeneeche->next;
			nodeneeche = nodeneeche->next;
		}


		while (nodeneeche != nullptr) {
			nodeooper->down = nullptr;
			nodeooper = nodeneeche->next;
		}


		while (nodeneeche != nullptr) {
			nodeneeche->up = nullptr;
			nodeneeche = nodeneeche->next;
		}



		lineAbove->down = line;
		line->up = lineAbove;
	}

	//recursively checks if backspacing cretes overflow condition

	void recursiveBackspacing(Line* currentLine) {
		if (!currentLine) return;




		Line* downline;
		if (currentLine->down)
		{
			downline = (currentLine->down);
		}
		else
		{
			return;
		}
		while (downline)
		{
			if (downline->overflow)
			{
				this->backspaceKaOverflow(downline);


				int currentlinelen = downline->getlength(); //this line throws exception
				int previousLinlen = downline->up->getlength();

				if (previousLinlen + currentlinelen + 1 <= 94) {

					Line* tempLine = linehead;
					int currentIndex = 1;
					while (tempLine && tempLine != downline) {
						tempLine = tempLine->down;
						currentIndex++;
					}
					//adda space in the downline->up at this point 
					// Call moveWordsToPreviousLine to move all remaining words up


					Node* spaceNode = new Node(' ');

					spaceNode->next = nullptr;
					Node* ln = downline->up->head;
					while (ln->next)
					{
						ln = ln->next;
					}
					ln->next = spaceNode;
					spaceNode->prev = ln;
					downline->up->charcount++;
					moveWordsToPreviousLine(currentIndex);
					return;


				}
				downline = downline->down;
			}
			else
			{
				return;
			}
		}
	}

	void clear() {
		while (linehead) {
			Line* temp = linehead;
			linehead = linehead->down;
			temp->deleteLine();
		}
	}


};
int consoleWidth, consoleHeight;
int mainTextHeight, suggestionHeight, searchWidth;

// Function to move the cursor to a specific position
void gotoxy(int x, int y) {
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Node* getWordStartingNode(Mylinkedlist& abc, int y, int x) {
//	Line* myline = abc.getLineAt(y);
//	Node* current = myline->head;
//	int column = 0;
//
//
//	while (current->next != nullptr && column < x) {
//		current = current->next;
//		column++;
//	}
//
//
//	while (current != nullptr && current->prev != nullptr && current->prev->c != ' ') {
//		current = current->prev;
//	}
//
//	return current;
//}
//gives the starting node in each line
Node* getWordStartingNode(Mylinkedlist& abc, int y, int x) {
	Node* current = abc.getLineAt(y)->bringnode(x);
	while (current->prev != nullptr && current->prev->c != ' ') {
		current = current->prev;
	}
	return current;
}
//for calculating the word lenght
int calculateWordLength(Node* wordHead) {
	int length = 0;
	Node* current = wordHead;


	while (current != nullptr && current->c != ' ' && current->c != '\n') {
		length++;
		current = current->next;
	}

	return length;
}

Node* getsnodefordel(Line* line, int startCol)
{
	Node* current = line->head;
	int count = 0;


	while (current != nullptr && count < startCol - 1) {
		current = current->next;
		count++;
	}


	Node* startNode = current;
	while (startNode != nullptr && startNode->c != ' ' && startNode->prev != nullptr) {
		startNode = startNode->prev;
	}


	return startNode->next;
}
bool myisalnum(char c) {
	// Check if the character is a letter (uppercase or lowercase) or a digit
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
		return true;
	}
	return false;
}
bool wordkbeechmei(Line* line, int col) {
	Node* current = line->head;


	for (int i = 1; i < col && current != nullptr; i++) {
		current = current->next;
	}


	if (current && myisalnum(current->c)) {

		return true;
	}


	if (current && current->prev && myisalnum(current->prev->c)) {

		return true;
	}

	return false;
}

struct info
{
	char type;
	Node* firstchar;
	int row;
	int col;
	info* nextin;
	int wordLength;
	Node* add;
	info(char typ, Node* head, int r, int c, int len, Node* e = nullptr) {
		type = typ;
		firstchar = head;
		row = r;
		col = c;
		nextin = nullptr;
		wordLength = len;
		add = e;
	}
};
Node* duplicateWord(Node* wordHead, int wordLength);
class mystack
{
public:
	info* top;
	int count;
	int limit;
	mystack() : top(nullptr), count(0), limit(5) {}

	~mystack() {
		while (top != nullptr) {
			pop();
		}
	}
	bool isEmpty() {
		return top == nullptr;
	}

	void removeOldest() {
		if (isEmpty()) return;

		info* current = top;
		info* previous = nullptr;


		while (current->nextin != nullptr) {
			previous = current;
			current = current->nextin;
		}


		if (previous != nullptr) {
			delete current;
			previous->nextin = nullptr;
		}
		else {

			delete top;
			top = nullptr;
		}
		count--;
	}
	//push each word into the stack which holds max 5 words
	void push(char actionType, Node* wordHead, int r, int c, int len, Node* e = nullptr) {

		if (actionType == 'i')
		{
			Node* temp = wordHead;
			while (temp)
			{
				temp = temp->next;
			}
			if (temp)
			{
				if (temp->c != ' ')
				{
					c++;
				}
			}

		}
		info* newAction = new info(actionType, wordHead, r, c, len, e);

		newAction->nextin = top;
		top = newAction;
		count++;


		if (count > limit) {
			removeOldest();
		}
	}
	void push(info* la, Mylinkedlist abc)
	{

		int calrows = 0;


		if (la->type == 'i')
		{

			calrows = la->col - la->wordLength;
			if (calrows == 0)
			{
				calrows = 1;
			}
		}
		else if (la->type == 'D')
		{
			calrows = la->col + la->wordLength + 1;
		}
		Node* duplicatedWordHead = duplicateWord(la->firstchar, la->wordLength);
		info* newAction = new info(la->type, duplicatedWordHead, la->row, calrows, la->wordLength);

		newAction->nextin = top;
		top = newAction;
		count++;


		if (count > limit) {
			removeOldest();
		}
	}
	//pops the latest word from the stack
	info* pop() {
		if (isEmpty()) {
			return nullptr;
		}

		info* temp = top;
		top = top->nextin;
		count--;
		return temp;
	}
	void removeInsertionAction(Node* wordHead) {
		if (isEmpty()) return;

		info* current = top;
		info* previous = nullptr;

		while (current != nullptr) {
			if (current->type == 'i' && current->firstchar->c == wordHead->c) {

				if (previous != nullptr) {
					previous->nextin = current->nextin;
				}
				else {

					top = current->nextin;
				}


				count--;
				return;
			}

			previous = current;
			current = current->nextin;
		}
	}

};
mystack def;
mystack redoStack;


bool lastcharspace = false;
void input(Mylinkedlist& abc, int y, int x, char ch) {

	abc.insert(y, x, ch);


	if (lastcharspace && ch == ' ') {
		return;
	}

	if (ch == ' ') {
		lastcharspace = true;  // Markin that a space was just entered
	}
	else {
		lastcharspace = false;
	}
	if (ch == '\n')
	{
		return;
	}


	Line* currentLine = abc.getLineAt(y);
	if (currentLine->EndOfLine(x)) {
		Node* current = currentLine->bringnode(x);


		while (current->prev != nullptr && current->prev->c != ' ') {
			current = current->prev;
		}


		Node* wordHead = current;
		int wordLength = calculateWordLength(wordHead);



		if (def.isEmpty() || (def.top->firstchar != wordHead)) {

			def.push('i', wordHead, y, x, wordLength);
		}
		else {

			int currentLineLength = currentLine->getlength();


			if (currentLineLength + wordLength > 94) {
				def.top->row = y + 1;
				def.top->col = 0;
			}
			else {
				def.top->row = y;
				def.top->col = x;
			}


			def.top->firstchar = wordHead;
			def.top->wordLength = wordLength;
			def.top->type = 'i';  // action type for insertion
		}
	}
	else if (ch == ' ')
	{
		Node* wordhead = getWordStartingNode(abc, y, x - 1);

		if (wordhead != nullptr && wordhead->c != ' ') {
			Node* wordHead = getWordStartingNode(abc, y, x - 1);
			int wordLength = calculateWordLength(wordHead);
			def.push('i', wordHead, y, x, wordLength);
		}
		/*int wordlen = calculateWordLength(wordhead);
		def.push('i', wordhead, y, x - 1, wordlen);*/
	}





}


// Function to set up the window size and calculate the layout areas
void setupWindow() {

	consoleWidth = 120;
	//consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	consoleHeight = 30;


	// Calculate the height of each section
	mainTextHeight = static_cast<int>(consoleHeight * 0.7);  // 60% of screen height
	suggestionHeight = static_cast<int>(consoleHeight * 0.2); // 20% of screen height
	searchWidth = static_cast<int>(consoleWidth * 0.2);      // 20% of screen width
}

void drawBorders() {

	for (int i = 0; i < consoleWidth; i++) {
		gotoxy(i, 0);
		cout << '-';
	}


	for (int i = 0; i < consoleWidth; i++) {
		gotoxy(i, consoleHeight - 1);
		cout << '-';
	}


	for (int i = 0; i < consoleHeight; i++) {
		gotoxy(0, i);
		cout << '|';
	}


	for (int i = 0; i < consoleHeight; i++) {
		gotoxy(consoleWidth - 1, i);
		cout << '|';
	}


	for (int i = 0; i < consoleHeight; i++) {
		gotoxy(consoleWidth - searchWidth, i);
		cout << '|';
	}


	for (int i = 0; i < consoleWidth; i++) {
		gotoxy(i, mainTextHeight);
		cout << '-';
	}


	for (int i = 0; i < consoleWidth; i++) {
		gotoxy(i, consoleHeight - suggestionHeight - 1);
		cout << '-';
	}


	gotoxy(consoleWidth - searchWidth + 1, 0);
	cout << "Search";

	gotoxy(1, mainTextHeight + 1);
	cout << "Word Suggestions";
}
int wordstart = 1;
void Line::moveToLine(Line* dest, int akhiriSpace) {

	Node* current = this->nodelkeao(akhiriSpace);
	Node* prev = this->nodelkeao(akhiriSpace - 1);


	if (current != nullptr) {

		if (prev != nullptr) {
			prev->next = nullptr;
		}
		else {
			this->head = nullptr;
		}

		current->prev = nullptr;


		if (dest->getlength() == 0) {
			dest->head = current;
		}
		else {

			Node* destHead = dest->head;
			Node* temp = current;

			while (temp->next != nullptr) {
				temp = temp->next;
			}


			temp->next = destHead;
			if (destHead != nullptr) {
				destHead->prev = temp;
			}
			//step 4
			if (temp->c != ' ' && destHead != nullptr && destHead->c != ' ') {
				Node* spaceNode = new Node(' ');

				temp->next = spaceNode;
				spaceNode->prev = temp;

				spaceNode->next = destHead;
				destHead->prev = spaceNode;


				temp = spaceNode;
				this->charcount++;
			}
			else {
				temp->next = destHead;
				if (destHead != nullptr) {
					destHead->prev = temp;
				}
			}


			dest->head = current;
		}



		Node* nodeInOldLine = current;
		Line* nextLine = dest->down;


		while (nodeInOldLine != nullptr) {

			if (nextLine != nullptr && nextLine->head != nullptr) {
				Node* correspondingNodeInNextLine = nextLine->head;
				int steps = 0;


				while (steps < akhiriSpace && correspondingNodeInNextLine != nullptr) {
					correspondingNodeInNextLine = correspondingNodeInNextLine->next;
					steps++;
				}


				if (correspondingNodeInNextLine != nullptr) {
					nodeInOldLine->down = correspondingNodeInNextLine;
					correspondingNodeInNextLine->up = nodeInOldLine;
				}
			}


			nodeInOldLine = nodeInOldLine->next;
		}
	}
}
void Mylinkedlist::removeEmptyLine(int lineIndex) {
	Line* current = linehead;


	for (int i = 1; i < lineIndex && current->down != nullptr; i++) {
		current = current->down;
	}


	if (current != nullptr) {
		Line* lineAbove = current->up;
		Line* lineBelow = current->down;


		if (lineAbove != nullptr) {
			lineAbove->down = lineBelow;
		}
		if (lineBelow != nullptr) {
			lineBelow->up = lineAbove;
		}


		Node* nodeCurrent = current->head;
		if (nodeCurrent != nullptr) {
			Node* nodeAbove = lineAbove ? lineAbove->head : nullptr;
			Node* nodeBelow = lineBelow ? lineBelow->head : nullptr;


			while (nodeCurrent != nullptr) {

				if (nodeAbove != nullptr) {
					nodeCurrent->up = nodeAbove;
					nodeAbove->down = nodeCurrent;
					nodeAbove = nodeAbove->next;
				}
				nodeCurrent = nodeCurrent->next;
			}
		}


		delete current;
		currentlines--;

		if (currentlines == 0)
		{
			linehead = nullptr;
		}
		/*if (lineBelow->down != nullptr) {
			removeEmptyLine(lineIndex);
		}*/
	}
}


void handleLinekaOverflow(Mylinkedlist& abc, int y, int& x, int totlen, int akhiriSpace) {

	//aage line nahi thi line banao move karo
	if (abc.getLineAt(y + 1) == nullptr) {

		abc.getLineAt(y)->splitLine(akhiriSpace);
		abc.getLineAt(y + 1)->overflow = true;
		abc.currentlines++;

	}
	//aage line thi

	else {

		int nextLineLength = abc.getlinelength(y + 1);
		//line khali thi move kardo
		if (nextLineLength == 0) {

			abc.getLineAt(y)->moveToLine(abc.getLineAt(y + 1), akhiriSpace);
		}
		//line khali nai thi abb overflow dekho

		else {
			//oveerflow nai hai go make new line
			if (!abc.getLineAt(y + 1)->overflow)
			{
				abc.getLineAt(y)->splitLine(akhiriSpace);
				abc.getLineAt(y + 1)->overflow = true;

			}
			else
			{
				//do prpeemd wala kaam
				Line* oldLine = abc.getLineAt(y);
				Line* newLine = abc.getLineAt(y + 1);


				oldLine->moveToLine(newLine, akhiriSpace);


				//if (abc.getlinelength(y + 1) >= consoleWidth - searchWidth) {
				if (abc.getlinelength(y + 1) >= 94) {

					handleLinekaOverflow(abc, y + 1, x, abc.getlinelength(y + 1), abc.getlinelength(y + 1) - 1);
				}
			}


		}
	}
}


//undo ka kammm///////////////////////////////////////////////
Node* duplicateWord(Node* wordHead, int wordLength) {
	if (!wordHead) return nullptr;


	Node* newHead = new Node(wordHead->c);
	Node* currentOriginal = wordHead->next;
	Node* currentNew = newHead;


	for (int i = 1; i < wordLength && currentOriginal != nullptr; ++i) {
		Node* newNode = new Node(currentOriginal->c);


		currentNew->next = newNode;
		newNode->prev = currentNew;


		currentNew = newNode;
		currentOriginal = currentOriginal->next;
	}

	return newHead;
}
bool compareWords(Node* word1, Node* word2) {
	while (word1 != nullptr && word2 != nullptr) {
		if (word1->c != word2->c) {
			return false;
		}
		word1 = word1->next;
		word2 = word2->next;
	}
	return (word1 == nullptr && word2 == nullptr);
}

//undo functionality implementation
void undo(Mylinkedlist& abc) {
	if (def.isEmpty()) {

		return;
	}


	info* lastAction = def.pop();
	redoStack.push(lastAction, abc);

	if (lastAction->type == 'i') {

		Node* wordHead = lastAction->firstchar;
		int row = lastAction->row;
		int col = lastAction->col;

		int len = lastAction->wordLength;
		int temp = len;

		Line* currentLine = abc.getLineAt(row);
		Node* check = currentLine->nodelkeao(col);


		for (int i = 0; i < temp + 1; i++) {

			currentLine->nodedeletion(col--);

		}

		//x = col+1;
		x = x - len - 1;
		y = row;
		gotoxy(x, y);


		//overflow
		if (abc.getLineAt(y)->isempty() && abc.currentlines > 1)
		{
			x = abc.getlinelength(y - 1);
			abc.removeEmptyLine(y);
			y--;
			gotoxy(x, y);
		}
		else if (abc.getLineAt(y)->isempty() && abc.currentlines == 1)
		{
			abc.removeEmptyLine(y);
			x = 1;
			y = 1;
			gotoxy(x, y);
		}
		//enter 
		else
		{
			if (row + 1 <= abc.currentlines)
			{
				if (abc.getLineAt(lastAction->row + 1)->isempty())
				{
					abc.removeEmptyLine(lastAction->row + 1);
				}
			}
		}


	}
	else if (lastAction->type == 'D') {

		Node* wordHead = lastAction->firstchar;
		int row = lastAction->row;
		int col = lastAction->col;
		int len = lastAction->wordLength;


		Line* currentLine = abc.getLineAt(row);
		if (!currentLine) {

			abc.appendLine();
			currentLine = abc.getLineAt(row);
		}

		Node* currentNode = wordHead;
		for (int i = 0; i < len; i++) {
			currentLine->insertion(currentNode->c, col);
			currentNode = currentNode->next;
			col++;
		}
		currentLine->insertion(' ', col);
		col++;
		//x=col
		x = col - 1;
		y = row;
		gotoxy(x, y);

	}
	else if (lastAction->type == 'M') {

		Node* wordHead = lastAction->firstchar;
		int row = lastAction->row;
		int col = lastAction->col;
		int len = lastAction->wordLength;


		Line* currentLine = abc.getLineAt(row);


		for (int i = 0; i < len; i++) {
			currentLine->nodedeletion(col);
		}


		Node* currentNode = wordHead;
		for (int i = 0; i < len; i++) {
			currentLine->insertion(col, currentNode->c);
			currentNode = currentNode->next;
			col++;
		}


	}


	delete lastAction;
}
//redo functonality implementation
void redo(Mylinkedlist& abc) {

	if (redoStack.isEmpty()) {
		return;
	}


	info* lastAction = redoStack.pop();

	if (lastAction->type == 'i') {

		Node* wordHead = lastAction->firstchar;
		int row = lastAction->row;
		int col = lastAction->col;
		int len = lastAction->wordLength;

		Line* currentLine = abc.getLineAt(row);
		if (!currentLine) {

			abc.appendLine();
			currentLine = abc.getLineAt(row);
		}



		Node* currentNode = wordHead;
		for (int i = 0; i < len; i++) {
			currentLine->insertion(currentNode->c, col);
			currentNode = currentNode->next;
			col++;
		}
		currentLine->insertion(' ', col);
		col++;



		x = col - 1;
		y = row;
		gotoxy(x, y);

	}
	else if (lastAction->type == 'D') {

		Node* wordHead = lastAction->firstchar;
		int row = lastAction->row;
		int col = lastAction->col - 2;
		int len = lastAction->wordLength;

		Line* currentLine = abc.getLineAt(row);
		if (!currentLine) {

			abc.appendLine();
			currentLine = abc.getLineAt(row);
		}


		Node* check = currentLine->bringnode(col);
		if (check->c == ' ')
		{
			for (int i = 0; i < len + 1; i++) {

				currentLine->nodedeletion(col--);

			}
		}
		else
		{
			for (int i = 0; i < len; i++) {

				currentLine->nodedeletion(col--);

			}
		}

		x = x - len - 1;
		y = row;
		gotoxy(x, y);


		if (abc.getLineAt(y)->isempty() && abc.currentlines > 1) {
			x = abc.getlinelength(y - 1);
			abc.removeEmptyLine(y);
			y--;
			gotoxy(x, y);
		}
		else if (abc.getLineAt(y)->isempty() && abc.currentlines == 1) {
			abc.removeEmptyLine(y);
			x = 1;
			y = 1;
			gotoxy(x, y);
		}
	}

	//delete lastAction;
}

bool previouskeywasbackspace = false;
//int x = 1, y = 1;
//file handling functions 
void displayMenu();
void readFile(const fs::path& filename, Mylinkedlist& abc);
void writeFile(const fs::path& filename, Mylinkedlist& abc);
void placeCursorAtTextEnd(Mylinkedlist& abc);
void createNewFile(fs::path& filename);
fs::path currentFilePath;

int main(int argc, char* argv[]) {

	Mylinkedlist abc;
	bool working = true;

	while (working) {

		displayMenu();
		int choice;
		std::cin >> choice;


		switch (choice) {
		case 1:
			createNewFile(currentFilePath);
			working = false;
			break;
		case 2: {
			std::cout << "Enter the path of the file to load: ";
			fs::path loadPath;
			std::cin >> loadPath;
			currentFilePath = loadPath;
			readFile(loadPath, abc);
			working = false;
			break;
		}

		case 3:
			std::cout << "Exiting. Have a great day!\n";
			return 0;
		default:
			std::cout << "Invalid choice. Please try again.\n";
			break;
		}
	}


	system("cls");
	HANDLE rhnd = GetStdHandle(STD_INPUT_HANDLE);  // handle to read console

	DWORD Events = 0;     // Event count
	DWORD EventsRead = 0; // Events read from console

	bool Running = true;

	// Set up the window layout
	setupWindow();
	// Draw the borders for different sections
	drawBorders();

	//int x = 1, y = 1; // Initial cursor position
	gotoxy(x, y);     // Move the cursor to starting position

	//Mylinkedlist abc; // Create an instance of the text editor

	//initial menu loop
	abc.display();
	placeCursorAtTextEnd(abc);



	// Program's main loop
	while (Running) {

		// Get the system's current "event" count
		GetNumberOfConsoleInputEvents(rhnd, &Events);

		if (Events != 0) { // If something happened, handle events

			// Create event buffer the size of how many events
			INPUT_RECORD eventBuffer[200];

			// Fill the event buffer with the events and save count in EventsRead
			ReadConsoleInput(rhnd, eventBuffer, Events, &EventsRead);

			// Loop through the event buffer using the saved count
			for (DWORD i = 0; i < EventsRead; ++i) {

				// Check if event[i] is a key event & if so, is a press not a release
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown) {




					// handleing special keys like Enter, Arrow keys, Esc
					switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode) {

					case VK_LEFT:  // Left arrow key
						if (x > 1) {
							x--;
						}
						else if (y > 1) {
							y--;
							x = abc.getlinelength(y);
							x++;
						}
						gotoxy(x, y);
						break;

					case VK_RIGHT:
					{

						int lineLength = abc.getlinelength(y);


						if (x < lineLength) {
							x++;
						}

						else if (x == lineLength) {
							x++;
						}

						else if (abc.getNodeAt(y + 1, 1) != nullptr) {
							y++;
							x = 1;
						}

						gotoxy(x, y);
						break;
					}

					case VK_UP:  // Up arrow key
					{

						Line* prevLine = abc.getLineAt(y - 1);

						if (prevLine != nullptr) {
							y--;


							int prevLineLength = abc.getlinelength(y);


							if (prevLineLength == 0) {
								x = 1;
							}
							else if (x > prevLineLength) {
								x = prevLineLength;
							}
						}


						gotoxy(x, y);
						break;
					}

					case VK_DOWN:  // Down arrow key
					{
						Line* nextline = abc.getLineAt(y + 1);

						if (nextline != nullptr) {
							y++;


							int nextlinelength = abc.getlinelength(y);
							if (nextlinelength == 0) {

								x = 1;
							}
							else if (x > nextlinelength) {

								x = nextlinelength;
								x++;
							}
						}

						gotoxy(x, y);
						break;
					}
					case VK_RETURN:  // Enter key

						input(abc, y, x, '\n');
						y++;
						x = 1;

						gotoxy(x, y);
						break;
					case VK_ESCAPE:  // ESC key to exit
						char saveChoice;
						std::cout << "Do you want to save the existing file? (y/n): ";
						std::cin >> saveChoice;

						if (saveChoice == 'y' || saveChoice == 'Y') {

							writeFile(currentFilePath, abc);
						}
						Running = false;
						break;
					case VK_BACK:  // Backspace key
						if (x > 1) {

							Line* currentLine = abc.getLineAt(y);
							Node* currentNode = currentLine->bringnode(x - 1);
							if (currentNode->c != ' ' && !previouskeywasbackspace) {

								Node* wordHead = getWordStartingNode(abc, y, x);
								int wordLength = calculateWordLength(wordHead);
								char actionype = 'D';


								if (def.isEmpty() || (def.top->type != 'D')) {

									Node* duplicatedWordHead = duplicateWord(wordHead, wordLength);
									def.push('D', duplicatedWordHead, y, x - wordLength, wordLength, wordHead);
									def.removeInsertionAction(wordHead);
								}
								else
								{
									if (def.top->type == 'D')
									{
										Node* recentdeleted = def.top->firstchar;

										if (def.top->add != wordHead)
										{
											Node* duplicatedWordHead = duplicateWord(wordHead, wordLength);
											def.push('D', duplicatedWordHead, y, x - wordLength, wordLength, wordHead);
										}
									}
								}
							}
							else if (currentNode->c == ' ') {

								Node* wordHead = getWordStartingNode(abc, y, x - 1);
								int wordLength = calculateWordLength(wordHead);
								char actionType = 'D';


								if (wordHead != nullptr && wordHead->c != ' ') {

									if (def.isEmpty() || (def.top->type != actionType)) {

										Node* duplicatedWordHead = duplicateWord(wordHead, wordLength);
										def.push(actionType, duplicatedWordHead, y, x - wordLength, wordLength, wordHead);
										def.removeInsertionAction(wordHead);
									}
									else {

										if (def.top->type == actionType) {
											if (def.top->add != wordHead) {
												Node* duplicatedWordHead = duplicateWord(wordHead, wordLength);
												def.push(actionType, duplicatedWordHead, y, x - wordLength, wordLength, wordHead);
											}
										}
									}
								}
							}



							x--;
							abc.linekadeletion(y, x);
							abc.recursiveBackspacing(abc.getLineAt(y));

							gotoxy(x, y);

							previouskeywasbackspace = false;
						}
						else if (x == 1) {

							if (y > 1 && abc.getlinelength(y) > 0 && abc.getlinelength(y - 1) > 0 && (abc.getlinelength(y) + abc.getlinelength(y - 1) <= 95)) {

								int prevLineLength = abc.getlinelength(y - 1);
								abc.moveWordsToPreviousLine(y);

								y--;

								x = prevLineLength + 1;

								gotoxy(x, y);

								abc.recursiveBackspacing(abc.getLineAt(y));
								previouskeywasbackspace = false;
							}
							else if (y > 1 && abc.getlinelength(y) > 0 && abc.getlinelength(y - 1) > 0 && (abc.getlinelength(y) + abc.getlinelength(y - 1) > 95)) {
								int prevLineLength = abc.getlinelength(y - 1);


								abc.backspaceKaOverflow(abc.getLineAt(y));
								y--;
								x = prevLineLength + 1;
								gotoxy(x, y);
								abc.recursiveBackspacing(abc.getLineAt(y));
								previouskeywasbackspace = true;
							}
							else if (abc.getlinelength(y) == 0) {
								gotoxy(x, y);
								previouskeywasbackspace = true;
							}
						}


						if (x == 1 && abc.getlinelength(y) == 0 && previouskeywasbackspace) {
							abc.linepooridelete(y);
							y--;

							if (y >= 1) {
								int prevLineLength = abc.getlinelength(y);
								x = (prevLineLength > 0) ? prevLineLength + 1 : 1;
							}
							gotoxy(x, y);
							previouskeywasbackspace = false;
						}


						if (x == 1 && y > 1 && abc.getlinelength(y) > 0 && abc.getlinelength(y - 1) == 0) {
							abc.removeEmptyLine(y - 1);

							y--;
							x = 1;
							gotoxy(x, y);
						}
						break;
					case '1'://case for undo 
						undo(abc);  // Call the undo function


						break;
					case '2':
						redo(abc);
						break;
					default:

						char ch = eventBuffer[i].Event.KeyEvent.uChar.AsciiChar;

						if (ch == 32 || (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122)) {
							input(abc, y, x, ch);
							x++;

							if (x == 1 || abc.getNodeAt(y, x - 1)->c == ' ') {
								wordstart = x;
							}
							int totlen = abc.getlinelength(y);

							if (totlen >= consoleWidth - searchWidth) {

								//int akhiriSpace = x - 1; 
								int akhiriSpace = totlen - 1;

								while (akhiriSpace >= 0 && abc.getNodeAt(y, akhiriSpace) != nullptr && abc.getNodeAt(y, akhiriSpace)->c != ' ') {
									akhiriSpace--;
								}


								if (akhiriSpace > 0 && abc.getNodeAt(y, akhiriSpace) != nullptr) {


									handleLinekaOverflow(abc, y, x, totlen, akhiriSpace);
									//y++; 
									//x = 1; // reset x to start of the new line

									if (x > akhiriSpace)
									{
										x = totlen - akhiriSpace + 1;
										y++;
									}
									wordstart = 1; // reset word start

								}
							}


						}
						else if (ch == ' ') {
							wordstart = x; // Reset on space
						}
						break;

					}
					system("cls");
					drawBorders();
					gotoxy(1, 1);
					abc.display();

					gotoxy(x, y);
				}
			}
		}
	}

	return 0;
}

void displayMenu() {
	const int consoleWidth = 120; // Set console width
	const string title = "Welcome to My Notepad";
	const string instructions1 = "You can insert, delete, undo, and redo text.";
	const string instructions2 = "Press 1 for Undo, 2 for Redo.";
	const string option1 = "1. Create New File - Create a new file from scratch.";
	const string option2 = "2. Load Existing File - Load an existing file to edit.";
	const string option3 = "3. Exit - Exit the application.";


	cout << "+" << string(consoleWidth - 2, '-') << "+" << endl;


	cout << "|";
	int padding = (consoleWidth - title.length() - 2) / 2;
	cout << string(padding, ' ') << title << string(consoleWidth - padding - title.length() - 2, ' ') << "|" << endl;


	cout << "|";
	cout << string(consoleWidth - 2, ' ') << "|" << endl;


	cout << "|";
	padding = (consoleWidth - instructions1.length() - 2) / 2;
	cout << string(padding, ' ') << instructions1 << string(consoleWidth - padding - instructions1.length() - 2, ' ') << "|" << endl;

	cout << "|";
	padding = (consoleWidth - instructions2.length() - 2) / 2;
	cout << string(padding, ' ') << instructions2 << string(consoleWidth - padding - instructions2.length() - 2, ' ') << "|" << endl;


	cout << "|";
	cout << string(consoleWidth - 2, ' ') << "|" << endl;


	cout << "+" << string(consoleWidth - 2, '-') << "+" << endl;


	cout << "|";
	padding = (consoleWidth - option1.length() - 2) / 2;
	cout << string(padding, ' ') << option1 << string(consoleWidth - padding - option1.length() - 2, ' ') << "|" << endl;

	cout << "|";
	padding = (consoleWidth - option2.length() - 2) / 2;
	cout << string(padding, ' ') << option2 << string(consoleWidth - padding - option2.length() - 2, ' ') << "|" << endl;

	cout << "|";
	padding = (consoleWidth - option3.length() - 2) / 2;
	cout << string(padding, ' ') << option3 << string(consoleWidth - padding - option3.length() - 2, ' ') << "|" << endl;


	cout << "|";
	cout << string(consoleWidth - 2, ' ') << "|" << endl;


	cout << "+" << string(consoleWidth - 2, '-') << "+" << endl;


	cout << "\nChoose an option: ";
}

//void readFile(const fs::path& filename, Mylinkedlist& abc) {
//	std::ifstream file(filename);
//	if (!fs::exists(filename)) {
//		//std::cout << "File does not exist. Creating a new file at " << loadPath << ".\n";
//		std::ofstream newFile(filename); // Create the file
//		if (newFile) {
//			std::cout << "New file created successfully.\n";
//		}
//		else {
//			std::cerr << "Failed to create the file.\n";
//		}
//		newFile.close();
//	}
//
//	if (!file.is_open()) {
//		std::cerr << "Unable to open file." << std::endl;
//		return;
//	}
//
//	char ch;
//	int row = 1;
//	int col = 1;
//
//	abc.appendLine(); // Assuming appendLine() initializes a new line in the linked list
//	while (file.get(ch)) {
//		if (ch == '\n') {
//
//			if (file.peek() != EOF) {
//				row++;
//				col = 1;
//				abc.appendLine(); // Append a new line for the next row
//			}
//		 // Append a new line for the next row
//		}
//		else {
//			abc.insert(row, col, ch); // Insert character into the specified position
//			col++;
//		}
//	}
//
//	file.close();
//
//}
//reading from file
void readFile(const fs::path& filename, Mylinkedlist& abc) {
	std::ifstream file(filename);
	if (!fs::exists(filename)) {
		std::ofstream newFile(filename);
		if (newFile) {
			std::cout << "New file created successfully.\n";
		}
		else {
			std::cerr << "Failed to create the file.\n";
		}
		newFile.close();
	}

	if (!file.is_open()) {
		std::cerr << "Unable to open file." << std::endl;
		return;
	}

	char ch;
	Line* currentLine = abc.linehead;
	int col = 1;
	int row = 1;
	int wordStart = 1;

	if (!currentLine) {
		abc.appendLine();
		currentLine = abc.linehead;
	}

	while (file.get(ch)) {

		if (ch == '\n') {
			if (file.peek() != EOF) {
				abc.appendLine();
				currentLine = currentLine->down;
				col = 1;
				wordStart = 1;
				row++;
			}
		}

		else if (ch == 32 || (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122)) {
			abc.insert(row, col, ch);
			col++;


			if (col == 1 || abc.getNodeAt(row, col - 1)->c == ' ') {
				wordStart = col;
			}

			int totlen = abc.getlinelength(row);


			if (totlen >= 94) {
				int akhiriSpace = totlen - 1;


				while (akhiriSpace >= 0 && abc.getNodeAt(row, akhiriSpace) != nullptr &&
					abc.getNodeAt(row, akhiriSpace)->c != ' ') {
					akhiriSpace--;
				}


				if (akhiriSpace > 0 && abc.getNodeAt(row, akhiriSpace) != nullptr) {
					handleLinekaOverflow(abc, row, col, totlen, akhiriSpace);

					if (col > akhiriSpace) {
						col = totlen - akhiriSpace + 1;
						row++;
						currentLine = currentLine->down;
					}
					wordStart = 1;
				}
			}
		}
		else if (ch == ' ') {

			wordStart = col;
		}
	}

	file.close();
	std::cout << "File read and processed successfully.\n";
}



//writing intofile
void writeFile(const fs::path& filename, Mylinkedlist& abc) {



	std::ofstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Unable to open file." << std::endl;
		return;
	}


	Line* currentLine = abc.getLineAt(1);
	while (currentLine) {
		Node* currentNode = currentLine->head;
		while (currentNode) {
			file << currentNode->c;
			currentNode = currentNode->next;
		}
		file << '\n';
		currentLine = currentLine->down;
	}

	file.close();
	std::cout << "File saved successfully.\n";
}


void createNewFile(fs::path& filename) {
	std::cout << "Enter the path to create the new file: ";
	std::cin >> filename;


	std::ofstream newFile(filename);
	if (newFile) {
		std::cout << "New file created successfully at " << filename << ".\n";
		currentFilePath = filename;
	}
	else {
		std::cerr << "Failed to create the file at " << filename << ".\n";
	}

	newFile.close();
}
//cursor adjustment 
void placeCursorAtTextEnd(Mylinkedlist& editor) {
	int lineIndex = 1;
	Line* currentLine = editor.linehead;
	if (!editor.linehead)
	{
		gotoxy(1, 1);
		return;
	}


	while (currentLine != nullptr && currentLine->down != nullptr) {
		currentLine = currentLine->down;
		lineIndex++;
	}


	int charIndex = 1;

	Node* currentChar = currentLine->head;

	while (currentChar != nullptr && currentChar->next != nullptr) {
		currentChar = currentChar->next;
		charIndex++;
	}
	x = charIndex;
	y = lineIndex;
	x = editor.getlinelength(lineIndex) + 1;

	gotoxy(x, y);
}
