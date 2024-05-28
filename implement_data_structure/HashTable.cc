const int MAX_HASH_TABLE_LEN = 1024;

struct ListNode
{
    int _val;
    ListNode* _next;

    ListNode(int val) : _val(val), _next(nullptr) {}
    ListNode() : _val(0), _next(nullptr) {}
};

class HashTable
{
public:
    HashTable();
    ~HashTable();

    void insert(int value);
    bool count(int value);
    void remove(int value);
    void clear();
    void print();

private:
    ListNode _head[MAX_HASH_TABLE_LEN];
    int hashFunc(int key);
};

HashTable::HashTable()
{

}

HashTable::~HashTable()
{
    clear();
}

int HashTable::hashFunc(int value)
{
    return value % MAX_HASH_TABLE_LEN;
}

void HashTable::insert(int value)
{
    int index = hashFunc(value);
    ListNode* node = new ListNode(value);
    // TODO:???
    node->_next = _head[index]._next;
    _head[index]._next = node;
}

bool HashTable::count(int value)
{
    
}
