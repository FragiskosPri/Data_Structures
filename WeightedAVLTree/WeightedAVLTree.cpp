#include <iostream>
#include <limits>
#include <vector>

using namespace std;

// Changes the balance of the trees
const double a = 0.75;

// Simple function to return only an integer value
int inputInt(int min, int max, bool limit)
{
    int input;
    bool valid = false;
    while (!valid)
    {
        if (limit)
        {
            cout << "Please enter an integer between " << min << " and " << max << ": ";
        }
        else
        {
            cout << "Please enter a number: ";
        }

        if (cin >> input)
        {
            if (limit && input >= min && input <= max)
            {
                valid = true;
            }
            else if (!limit)
            {
                valid = true;
            }
        }

        if (!valid)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. ";
        }
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return input;
}

class Tree
{
public:
    // This represents a node(s) in a tree
    struct Node
    {
        int value;
        Node *left;
        Node *right;
        int height;

        Node(int val)
        {
            value = val;
            left = nullptr;
            right = nullptr;
            height = 1;
        }
    };

    Node *root;

    Tree()
    {
        this->root = NULL;
    }
    //------ Getters ------//
    // Get height of the current node
    int get_height(Node *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return node->height;
    }

    // Get size of the entire tree
    int get_size(Node *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return 1 + get_size(node->left) + get_size(node->right);
    }

    //------ Methods ------//
    void insert(int value)
    {
        bool inserted = false;
        root = insert_helper(root, value, a, inserted);
        if (inserted)
        {
            Order();
            cout << "Inserted " << value << endl;
        }
    }

    void Order()
    {
        inOrder(root);
    }


    void remove(int value)
    {
        bool removed = true;
        root = remove_helper(root, value, a, removed);
        if (removed)
        {
            Order();
            cout << "Removed " << value << endl;
        }
        else if (!removed)
        {
            cout << "Value " << value << " not found in the tree." << endl;
        }
    }

    // Basically, it finds all the nodes in the path and puts them in a vector
    Tree::Node *search(int value)
    {
        vector<int> series;
        Tree::Node *result = search_helper(root, value, series);
        if (result != nullptr)
        {
            for (int num : series)
            {
                cout << num << " -> ";
            }
            cout << value << endl;
        }
        else
        {
            cout << "Value not found in the tree." << endl;
        }
        return result;
    }

    void Find_Neighbor(int value)
    {
        int direction;
        cout << "1. Predecessor\n2. Successor" << endl;
        int selected = inputInt(1, 2, true);
        if (selected == 1)
        {
            Tree::Node *pred = findPredecessor_helper(root, value);
            if (pred != nullptr)
            {
                cout << "Predecessor of " << value << " is " << pred->value << "\n";
            }
            else
            {
                cout << "No predecessor found for " << value << "\n";
            }
        }
        else
        {
            Tree::Node *succ = findSuccessor_helper(root, value);
            if (succ != nullptr)
            {
                cout << "Successor of " << value << " is " << succ->value << "\n";
            }
            else
            {
                cout << "No successor found for " << value << "\n";
            }
        }
    }

private:
    // Private helper function for inserting a node into the tree
    Node *insert_helper(Node *node, int value, double a, bool &inserted)
    {
        // Check if the value already exists in the tree
        if (node != nullptr && node->value == value)
        {
            cout << "Node with value " << value << " already exists in the tree!" << endl;
            inserted = false; // Set inserted flag to false
            return node;
        }

        // Insertion logic here
        if (node == nullptr)
        {
            inserted = true; // Set inserted flag to true
            return new Node(value);
        }
        if (value < node->value)
        {
            node->left = insert_helper(node->left, value, a, inserted);
        }
        else
        {
            node->right = insert_helper(node->right, value, a, inserted);
        }

        // Update the height of the current node based on the heights of its children
        node->height = 1 + max(get_height(node->left), get_height(node->right));

        // Balancing logic here based on the a-balance property
        int left_size = get_size(node->left);   // Get the size (number of nodes) in the left subtree
        int right_size = get_size(node->right); // Get the size (number of nodes) in the right subtree

        if (left_size > a * get_size(node))
        {
            if (value < node->left->value)
            {
                return right_rotate(node);
            }
            else
            {
                node->left = left_rotate(node->left);
                return right_rotate(node);
            }
        }
        else if (right_size > a * get_size(node))
        {
            if (value > node->right->value)
            {
                return left_rotate(node);
            }
            else
            {
                node->right = right_rotate(node->right);
                return left_rotate(node);
            }
        }

        return node;
    }

    // Private helper function for deleting a node from the tree
    Tree::Node *remove_helper(Tree::Node *node, int value, double a, bool &removed)
    {
        if (node == nullptr)
        {
            removed = false; // Value not found in the tree
            return node;
        }

        if (value < node->value)
        {
            node->left = remove_helper(node->left, value, a, removed);
        }
        else if (value > node->value)
        {
            node->right = remove_helper(node->right, value, a, removed);
        }
        else
        {
            // Node found, perform removal logic
            if (node->left == nullptr && node->right == nullptr)
            { // If the node has no children
                delete node;
                return nullptr;
            }
            else if (node->left == nullptr)
            { // If the node only has a right child
                Node *temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            { // If the node only has a left child
                Node *temp = node->left;
                delete node;
                return temp;
            }
            else
            { // Node has both left and right children, find the in order successor
                Node *successor = findSuccessor_helper(node->right, value);
                node->value = successor->value;
                node->right = remove_helper(node->right, successor->value, a, removed);
            }
        }

        // Update the height of the current node based on the heights of its children
        node->height = 1 + max(get_height(node->left), get_height(node->right));

        // Balancing logic here based on the a-balance property
        int left_size = get_size(node->left);
        int right_size = get_size(node->right);

        if (left_size > a * get_size(node))
        {
            if (value < node->left->value)
            {
                return right_rotate(node);
            }
            else
            {
                node->left = left_rotate(node->left);
                return right_rotate(node);
            }
        }
        else if (right_size > a * get_size(node))
        {
            if (value > node->right->value)
            {
                return left_rotate(node);
            }
            else
            {
                node->right = right_rotate(node->right);
                return left_rotate(node);
            }
        }

        return node;
    }

    //----------------------------------------------------------------------------------------------------------------------------------------------------
    // Search node
    Tree::Node *search_helper(Tree::Node *node, int value, vector<int> &series)
    {
        if (node == nullptr || node->value == value)
        { // Node found
            return node;
        }

        if (value < node->value)
        { // Go to the left
            series.push_back(node->value);
            return search_helper(node->left, value, series);
        }
        else
        { // Go to the right
            series.push_back(node->value);
            return search_helper(node->right, value, series);
        }
    }

    // Simply puts and prints all the nodes in the tree in order
    void inOrder(Node *node)
    {
        if (node == NULL)
        {
            return;
        }
        inOrder(node->left);
        cout << node->value << " ";
        inOrder(node->right);
    }

    // Find predecessor node
    Node *findPredecessor_helper(Node *node, int value)
    {
        Node *predecessor = nullptr;
        while (node != nullptr)
        {
            if (node->value < value)
            {
                predecessor = node;
                node = node->right;
            }
            else
            {
                node = node->left;
            }
        }
        return predecessor;
    }

    // Find successor node
    Node *findSuccessor_helper(Node *node, int value)
    {
        Node *successor = nullptr;
        while (node != nullptr)
        {
            if (node->value > value)
            {
                successor = node;
                node = node->left;
            }
            else
            {
                node = node->right;
            }
        }
        return successor;
    }

    //------ Tree rotations ------//

    // Rotates the tree to the right
    Node *right_rotate(Node *node)
    {
        Node *new_parent = node->left;
        node->left = new_parent->right;
        new_parent->right = node;
        node->height = 1 + max(get_height(node->left), get_height(node->right));
        new_parent->height = 1 + max(get_height(new_parent->left), get_height(new_parent->right));
        return new_parent;
    }

    // Rotates the tree to the left
    Node *left_rotate(Node *node)
    {
        Node *new_parent = node->right;
        node->right = new_parent->left;
        new_parent->left = node;
        node->height = 1 + max(get_height(node->left), get_height(node->right));
        new_parent->height = 1 + max(get_height(new_parent->left), get_height(new_parent->right));
        return new_parent;
    }
};

int main()
{

    Tree tree = Tree();
    int selected;
    while (true)
    {
        cout << "What would you like to do?\n1. Insert a new node.\n2. Delete node.\n3. Find node.\n4. Find neighboring nodes.\n5. Exit\n";
        selected = inputInt(1, 5, true);
        switch (selected)
        {
        case 1: // Insert a new node
            cout << "What node would you like to insert? (insert 0 to exit)" << endl;
            selected = inputInt(1, 5, false);
            while (selected != 0)
            {
                tree.insert(selected);
                cout << "What node would you like to insert?" << endl;
                selected = inputInt(1, 5, false);
            }
            break;
        case 2: // Remove node
            selected = inputInt(1, 5, false);
            tree.remove(selected);
            break;

        case 3: // Search for node
            selected = inputInt(1, 5, false);
            tree.search(selected);
            break;

        case 4: // Find neighbors
            cout << "Enter the value for the node you are searching for:\n";
            selected = inputInt(1, 5, false);
            tree.Find_Neighbor(selected);
            break;
        default:
            return 0;
        }
    }
}