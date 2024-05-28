#include <cstdio>
#include <assert.h>
#include <iostream>
#include <vector>
using namespace std;

// #define DEBUG

enum RBTColor{RED, BLACK};

template <typename T>
class RBTNode
{
public:
    RBTNode() = default;
    RBTNode(T key, RBTColor color, RBTNode* parent, RBTNode* left, RBTNode* right)
    : _key(key),  _color(color), _parent(parent), _left(left), _right(right)
    {}

    T _key;
    RBTColor _color;
    RBTNode *_left;
    RBTNode *_right;
    RBTNode *_parent;
};

template <typename T>
class RBTree
{
public:
    RBTree();
    // TODO:
    ~RBTree();
    
    // TODO:前序遍历
    void preOrder();
    // 中序遍历
    void inOrder();
    // TODO:后序遍历
    void postOrder();

    // TODO:(递归实现)查找"红黑树"中键值为key的节点
    RBTNode<T>* search(T key);

    // TODO:查找最小结点：返回最小结点的键值。
    T minimum();
    // TODO:查找最大结点：返回最大结点的键值。
    T maximum();

    // TODO:找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
    RBTNode<T>* successor(RBTNode<T> *x);
    // TODO:找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
    RBTNode<T>* predecessor(RBTNode<T> *x);

    // TODO:销毁红黑树
    void destroy();

    // TODO:打印红黑树
    void print();

    void insert(T key);
    void remove(T key);

private:

    // 创建新节点
    RBTNode<T>* createNode(const T& x = T());
    
    void inOrder(RBTNode<T>* node);
    
    RBTNode<T>* search(RBTNode<T>* root, T key);

    // 左旋
    void leftRotate(RBTNode<T>* node);
    // 右旋
    void rightRotate(RBTNode<T>* node);
    // 插入函数
    void insert(RBTNode<T>* node);
    // 插入修正函数
    void insertFixUp(RBTNode<T>* node);
    // 删除函数
    void remove(RBTNode<T> *node);
    // 删除修正函数
    void removeFixUp(RBTNode<T> *node, RBTNode<T> *parent);
    
    RBTNode<T>* _root;   // 根节点
    RBTNode<T>* _leaf;   // 指向空节点
};

// TODO:创建新节点
template <typename T>
RBTNode<T>* RBTree<T>::createNode(const T& x)
{
    RBTNode<T>* node = new RBTNode<T>();
    assert(node != nullptr);
    node->_color = RED;
    node->_left = node->_right = node->_parent = _leaf;
    node->_key = x;
    return node;
}

template <typename T>
RBTree<T>::RBTree()
{
    _leaf = createNode();
    _root = _leaf;
    _leaf->_color = BLACK;
}

template <typename T>
RBTree<T>::~RBTree()
{
    
}

template <typename T>
void RBTree<T>::leftRotate(RBTNode<T>* node)
{
    RBTNode<T>* nRight = node->_right;

    // 1.
    node->_right = nRight->_left;
    if (nRight->_left != _leaf) {
        nRight->_left->_parent = node;
    }

#ifdef DEBUG
    if (node != nullptr && node->_right == nullptr) {
        printf("左旋了一个叶子节点\n");
    }
#endif

    // 2.
    nRight->_parent = node->_parent;
    if (node->_parent == _leaf) {
        _root = nRight;
    } else {
        if (node == node->_parent->_left) {
            node->_parent->_left = nRight;
        } else {
            node->_parent->_right = nRight;
        }
    }

    // 3.
    nRight->_left = node;
    node->_parent = nRight;
}

template <typename T>
void RBTree<T>::rightRotate(RBTNode<T>* node)
{
    RBTNode<T>* nLeft = node->_left;

    // 1.
    node->_left = nLeft->_right;
    if (nLeft->_right != _leaf) {
        nLeft->_right->_parent = node;
    }

    // 2.
    nLeft->_parent = node->_parent;
    if (node->_parent == _leaf) {
        _root = nLeft;
    } else {
        if (node == node->_parent->_left) {
            node->_parent->_left = nLeft;
        } else {
            node->_parent->_right = nLeft;
        }
    }

    // 3.
    nLeft->_right = node;
    node->_parent = nLeft;
}

// 属于外部接口
template <class T>
void RBTree<T>::insert(T key)
{
    RBTNode<T> *newNode = _leaf;

    // 如果新建结点失败，则返回。
    if ((newNode = new RBTNode<T>(key, BLACK, _leaf, _leaf, _leaf)) == nullptr)
        return ;
    
    // 插入节点
    insert(newNode);
}

template <typename T>
void RBTree<T>::insert(RBTNode<T>* node)
{
    RBTNode<T>* pre = _leaf;
    RBTNode<T>* cur = _root;

    // 找到插入位置的前驱节点
    while (cur != _leaf) 
    {
        pre = cur;
        if (node->_key < cur->_key) {
            cur = cur->_left;
        } else {
            cur = cur->_right;
        }
    }

    // 设置插入节点的父节点
    node->_parent = pre;
    if (pre != _leaf) 
    {
        // 插入节点作为其前驱节点的左孩子或右孩子
        if (node->_key < pre->_key) {
            pre->_left = node;
        } else  {
            pre->_right = node;
        }
    } 
    else 
    {
        // 如果前驱节点为空，说明红黑树为空，则插入节点作为root节点
        _root = node;
        _root->_parent = _leaf;
    }

    // 设置插入节点的颜色为红色（设置为黑色则一定回破坏黑高，则一定会旋转，红色不一定）
    node->_color = RED;

    // 重新修正
    insertFixUp(node);
}

// 插入操作之后维护红黑树
template <class T>
void RBTree<T>::insertFixUp(RBTNode<T>* node)
{
    // 插入节点的父亲节点
    RBTNode<T>* parent = _leaf;
    // 插入节点的祖父节点
    RBTNode<T>* gparent = _leaf;

    // 父亲节点为红色才需要维护，黑色不需要
    // 因为相邻两个节点不能为红色
    while ((parent = node->_parent) && (parent->_color == RED))
    {
        gparent = parent->_parent;
        if (gparent == _leaf) {
            printf("gparent == _leaf\n");
            return;
        }    

        // 若父节点是祖父节点的左孩子
        if (parent == gparent->_left)
        {
            // 父节点是祖父节点的左孩子，自然叔父节点就是祖父节点的右孩子
            RBTNode<T>* uncle = gparent->_right;
            // 1.如果叔父节点是红色
            if (uncle != _leaf && uncle->_color == RED)
            {
                uncle->_color = BLACK;
                parent->_color = BLACK;
                gparent->_color = RED;
                // 向上调整
                node = gparent;
                continue;
            } 
            // 如果叔父节点是黑色
            else 
            { 
                // 2.叔父节点是黑色，且插入节点是右孩子
                if (node == parent->_right)
                {
                    RBTNode<T>* tmp;
#ifdef DEBUG
                    if (node == nullptr) printf("failed()\n");
#endif
                    leftRotate(parent);
                    tmp = parent;
                    parent = node;
                    node = tmp;
                }
                // 3.叔父节点是黑色，且插入节点是左孩子
                parent->_color = BLACK;
                gparent->_color = RED;
                rightRotate(gparent);
            } 
        }
        // 若父节点是祖父节点的右孩子
        else 
        {
            // 自然，叔父节点是祖父节点的左孩子
            RBTNode<T>* uncle = gparent->_left;
            // 1.叔父节点是红色
            if ((uncle != _leaf) && (uncle->_color == RED))
            {
                uncle->_color = BLACK;
                parent->_color = BLACK;
                gparent->_color = RED;
                // 向上调整
                node = gparent;
                continue;
            }

            // 2.叔父节点是黑色，且当前节点是左孩子
            if (parent->_left == node)
            {
                RBTNode<T>* tmp;
                rightRotate(parent);
                // TODO:可以重载swap函数
                tmp = parent;
                parent = node;
                node = tmp;
            }
            // 3.叔父节点是黑色，且当前节点是右孩子
            parent->_color = BLACK;
            gparent->_color = RED;
#ifdef DEBUG
            if (node == nullptr) printf("failed()\n");
#endif
            leftRotate(gparent);
        }
    }
    // 将根节点设置为黑色
    _root->_color = BLACK;
}

template <class T>
RBTNode<T>* RBTree<T>::search(T key)
{
    return search(_root, key);
}

template <class T>
RBTNode<T>* RBTree<T>::search(RBTNode<T>* root, T key)
{  
    if (root == _leaf) {
        return _leaf;
    }

#ifdef DEBUG
    if (root == nullptr) 
        printf("search failed, root == nullptr\n");
    printf("node->key = %d\n", root->_key);
#endif
    if (root->_key == key) {
        return root;
    }

    if (key < root->_key) {
        return search(root->_left, key);
    } else {
        return search(root->_right, key);
    }

}

template <class T>
void RBTree<T>::remove(T key)
{
    // 先根据key找到节点，然后再传入删除节点
    RBTNode<T>* target;
    if ((target = search(key)) != _leaf)
    {
        remove(target);
    }
    else
    {
        printf("key is not exist.\n");
    }
}

// 删除红黑树中键值为key的节点
// template <class T>
// void RBTree<T>::remove(T key)
// {
//     RBTNode<T> *node; 

//     // 查找key对应的节点(node)，找到的话就删除该节点
//     if ((node = search(_root, key)) != nullptr)
//         remove(_root, node);
// }

template <class T>
void RBTree<T>::remove(RBTNode<T> *node)
{
    RBTNode<T>* child;
    RBTNode<T>* parent;

    RBTColor color;

    // 被删除节点的左右孩子都不为空的情况
    if ((node->_left != _leaf) && (node->_right != _leaf))
    {
        RBTNode<T>* replace = node;

        // 获取后继节点
        // 寻找右子树最左的节点，即「大于node的最小节点」
        // 这样才符合二叉搜索树的特性
        replace = replace->_right;
        while (replace->_left != _leaf) {
            replace = replace->_left;
        }

        // 将node节点更新为replace节点，其父节点指向需要更新
        if (node != _root)
        {
            if (node->_parent->_left == node) {
                node->_parent->_left = replace;
            } else {
                node->_parent->_right = replace;
            }
        }
        else 
        {
            // node节点是根节点，更新根节点为「替换节点」
            _root = replace;
        }

        child = replace->_right;
        parent = replace->_parent;
        // 保存「取代节点」的颜色
        color = replace->_color;

        // 「被删除节点」是「它的替代及节点的父节点」
        //  TODO:什么情况发生？
        // 找到的replace节点就是node的右儿子
        if (parent == node)
        {
            parent = replace;
        } 
        else 
        {
            // 替代节点的右子树不为空
            // 将其变成其祖父节点的左儿子(已经确定找到的replace节点是左孩子)
            if (child != _leaf) {
                child->_parent = parent;
            }
            parent->_left = child;

            replace->_right = node->_right;
            node->_right->_parent = replace;
        }
        
        // replace节点替代node节点的位置和属性
        replace->_parent = node->_parent;
        replace->_color = node->_color;
        replace->_left = node->_left;
        node->_left->_parent = replace;

        // 我们要删除「替代节点」，如果是黑色就影响了黑高，需要调整
        if (color == BLACK) {
            removeFixUp(child, parent);
        }
        delete node;
        return;
    } // 被删除节点的左右孩子都不为空的情况

    // 被删除节点的左孩子不为空
    if (node->_left != _leaf) 
    {
        child = node->_left;
    } 
    // 被删除节点的左孩子为空，右孩子不确定
    else 
    { 
        child = node->_right;
    }

    parent = node->_parent;
    // 保存「取代节点」的颜色
    color = node->_color;

    if (child != _leaf) {
        child->_parent = parent;
    }

    // node节点不是根节点
    if (parent)
    {
        if (parent->_left == node) {
            parent->_left = child;
        } else {
            parent->_right = child;
        }    
    } else {
        _root = child;
    }
    if (color == BLACK) {
        removeFixUp(child, parent);
    }
        
    delete node;
}

template <class T>
void RBTree<T>::removeFixUp(RBTNode<T> *node, RBTNode<T> *parent)
{
    // 被删除节点的兄弟节点
    RBTNode<T>* brother;

    while ((node != _leaf || node->_color == BLACK) && node != _root)
    {
        // 如果被删除节点是左孩子
        if (parent->_left == node)
        {
            brother = parent->_right;
            // 1.如果兄弟节点是红色
            if (brother->_color == RED)
            {
                brother->_color = BLACK;
                parent->_color = RED;
                leftRotate(parent);
                brother = parent->_right;
            }
            // 2.如果兄弟结点是黑色的，而且兄弟结点的两个孩子结点都是黑色的
            if ((brother->_left != _leaf || brother->_left->_color == BLACK) &&
                (brother->_right != _leaf || brother->_right->_color == BLACK))
            {
                brother->_color = RED;
                node = parent;
                parent = node->_parent;
            }
            else 
            {
                // 3.兄弟节点是黑色的，并且兄弟节点的左孩子是红色，右孩子为黑色
                if (brother->_right != _leaf || brother->_right->_color == BLACK)
                {
                    brother->_left->_color = BLACK;
                    brother->_color = RED;
                    rightRotate(brother);
                    brother = parent->_right;
                }
                // 4.兄弟节点是黑色的，并且兄弟节点的右孩子是红色，左孩子为黑色
                brother->_color = parent->_color;
                parent->_color = BLACK;
                brother->_right->_color = BLACK;
                leftRotate(parent);
                node = _root;
                break;
            }

        } // 如果被删除节点是左孩子

        // 如果被删除节点是右孩子
        else 
        {
            brother = parent->_left;
            if (brother->_color == RED)
            {
                brother->_color = BLACK;
                parent->_color = RED;
                rightRotate(parent);
                brother = parent->_left;
            }
            if ((brother->_left != _leaf || brother->_left->_color == BLACK) &&
                (brother->_right != _leaf || brother->_right->_color == BLACK))
            {
                brother->_color = RED;
                node = parent;
                parent = node->_parent;
            }
            else 
            {
                if (brother->_left != _leaf || brother->_left->_color == BLACK)
                {
                    brother->_right->_color = BLACK;
                    brother->_color = RED;
                    leftRotate(brother);
                    brother = parent->_left;
                }
                brother->_color = parent->_color;
                parent->_color = BLACK;
                brother->_left->_color = BLACK;
                rightRotate(parent);
                node = _root;
                break;
            }

        }
    }
    if (node) {
        node->_color = BLACK;
    }
}

template <typename T>
void RBTree<T>::inOrder()
{
    inOrder(_root);
}

//中序遍历打印结点详细的结点颜色
template <typename T>
void RBTree<T>::inOrder(RBTNode<T>* node)
{
    if (node == _leaf) return;
    if (node->_left != _leaf) inOrder(node->_left);
    cout << node->_key << "(" << ((node->_color == BLACK) ? "BLACK" : "RED") << ")" << " ";
    if (node->_right != _leaf) inOrder(node->_right);
}

int main()
{
    printf("Hello World!\n");
    RBTree<int> rbtree;
    vector<int> vec{20, 100, 30, 10, 60, 90, 40, 80, 50, 70};
    for (const auto& i : vec) 
    {
        rbtree.insert(i);
    }
    rbtree.inOrder();
    cout << endl;

    rbtree.remove(10);
    rbtree.remove(30);
    rbtree.remove(50);
    rbtree.remove(100);

    rbtree.inOrder();
    cout << endl;
    
    auto it = rbtree.search(20);
    cout << it->_key << endl;

    it = rbtree.search(60);
    cout << it->_key << endl;

    // 找不到，返回_leaf
    it = rbtree.search(1000);
    cout << it->_key << endl;   // 0
    cout << it->_color << endl; // 1 BLACK

    return 0;
}