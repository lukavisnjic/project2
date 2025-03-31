#include "types.h"
#define MAX_PATH 256

extern struct NODE* root;
extern struct NODE* cwd;

struct NODE* splitPath(char* pathName, char* baseName, char* dirName);
struct NODE* findNode(struct NODE* parent, const char* name);


//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW
    
	char baseName[64];
	char dirName[MAX_PATH];

	size_t length = strlen(pathName);
	if (length > 1 && pathName[length - 1] == '/') {
		pathName[length - 1] = '\0';
	}

	// No Path Given
	if (strcmp(pathName, "/") == 0) {
		printf("MKDIR ERROR: no path provided\n");	
		return;
	}
	
	struct NODE* parent = splitPath(pathName, baseName, dirName);

	if (!parent) return;
	
	// Already Exists Check
	struct NODE* exists = findNode(parent, baseName);
	if (exists) {
		printf("MKDIR ERROR: directory %s already exists\n", baseName);
		return;
	}

	// New Dir NODE
	struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
	strcpy(newDir->name, baseName);
	newDir->fileType = 'D';
	newDir->childPtr = NULL;
	newDir->siblingPtr = NULL;
	newDir->parentPtr = parent;

	// Child Sibling Add
	if (!parent->childPtr) {
		parent->childPtr = newDir;
	}
	else {
		struct NODE* current = parent->childPtr;
		while (current->siblingPtr) current = current->siblingPtr;
		current->siblingPtr = newDir;
	}

	printf("MKDIR SUCCESS: node %s successfully created\n",pathName);
    return;
}

struct NODE* findNode(struct NODE* parent, const char* name) {
	if (!parent) return NULL;
	struct NODE* current = parent->childPtr;
	while (current) {
		if (strcmp(current->name,name) == 0) {
			return current;
		}
		current = current->siblingPtr;
	}
	return NULL;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    // TO BE IMPLEMENTED
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED SOLUTION EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

    // YOUR CODE HERE
   	char localPath[MAX_PATH];
	strcpy(localPath,pathName);
	
	// Empty of / or NULL pathName
	if (!pathName || strlen(pathName) == 0 || strcmp(pathName, "/") == 0) {
		strcpy(dirName, "/");
		strcpy(baseName,"");
		return root;
	}
	
	size_t length = strlen(localPath);
	if (length > 1 && localPath[length - 1] == '/') {
		localPath[length - 1] = '\0';
	}

	//Location for Path Split
	char* lastSlash = strrchr(localPath, '/');	

	if (lastSlash == NULL) {
		strcpy (dirName, "");
		strcpy(baseName, pathName);
		return cwd;
	}

	// Split the Path
	if (lastSlash == localPath) {
		strcpy (dirName, "/");
	}
	else {
		size_t len = lastSlash - localPath;
		strncpy(dirName, localPath, len);
		dirName[len] = '\0';
	}

	strcpy(baseName,lastSlash + 1);

	// Find parent directory node 
	struct NODE* current = (localPath[0] == '/') ? root : cwd;

	char dirCopy[MAX_PATH];
	strcpy(dirCopy, dirName);

	char* token = strtok(dirCopy, "/");
	while (token != NULL) {
		struct NODE* next = findNode(current, token);
		if (!next || next -> fileType != 'D') {
			printf("ERROR: directory %s does not exist\n", token);
			return NULL;
		}
		current = next;
		token = strtok(NULL, "/");
	}

    return current;
}
