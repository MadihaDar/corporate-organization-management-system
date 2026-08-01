#include "franchise.h"

// PURCHASE CLASS
Purchase::Purchase():purchaseID(""), date(""), items(nullptr), totalPaid(0.0) {}    // default constructor

Purchase::Purchase(const string& id, const string& d) : purchaseID(id), date(d), items(nullptr), totalPaid(0.0) {}  // parameterized constructor

Purchase::Purchase(const Purchase& other): purchaseID(other.purchaseID), date(other.date), items(nullptr), totalPaid(other.totalPaid) { // copy constructor
    Node<Product>* curr = other.items;
    Node<Product>* last = nullptr;

    while (curr) {
        Node<Product>* newNode = new Node<Product>(curr->getData());
        if (!items) items = newNode;
        else last->setNext(newNode);

        last = newNode;
        curr = curr->getNext();
    }
}

Purchase::~Purchase() { // destructor
    while(items) {
        Node<Product>* temp = items;
        items = items->getNext();
        delete temp;
    }
}

// getters
string Purchase::getPurchaseID() const { return purchaseID; }   
string Purchase::getDate() const { return date; }
Node<Product>* Purchase::getItems() const { return items; }
float Purchase::getTotalPrice() const {
    float sum = 0.0;
    Node<Product>* curr = items;
    while(curr) {
        const Product& p = curr->getData();
        sum += p.getProductPrice() * p.getProductSales();
        curr = curr->getNext();
    }
    return sum;
}

void Purchase::addItem(const Product& p, int quantity) {
    if(quantity <= 0) return;

    Product copy = p;
    copy.setProductSales(quantity); // set quantity for this purchase item

    Node<Product>* newNode = new Node<Product>(copy);   // create a new linke dlist
    
    // if this is the first item in the purchase
    if(!items) {
        items = newNode;
    } else {    // otherwise, append to the list's end
        Node<Product>* curr = items;
        while(curr->getNext()) curr = curr->getNext();
        curr->setNext(newNode);
    }

    totalPaid += p.getProductPrice() * quantity;    // update total price for this purchase
}

json Purchase::toJSON() const {
    json arr = json::array();
    Node<Product>* curr = items;
    while(curr) {   // iterate over all purchased products
        const Product& p = curr->getData();
        arr.push_back({
            {"productID", p.getProductID()},
            {"quantity", p.getProductSales()},
            {"price", p.getProductPrice()}
        });
        curr = curr->getNext();
    }

    return {
        {"purchaseID", purchaseID},
        {"date", date},
        {"products", arr},
        {"totalPaid", totalPaid}
    };
}

Purchase Purchase::fromJSON(const json& j) {
    Purchase p(j.at("purchaseID"), j.at("date"));  // purchase object with id and date
    for(auto& item : j["products"]) {   // iterate over each product in the JSON array
        Product prod(item.at("productID"), item.at("quantity"), item.at("price"));
        const int qty = item.at("quantity");
        p.addItem(prod, qty);
    }
    return p;
}

// CUSTOMER CLASS
Customer::Customer(string id = "") : customerID(id), purchaseList(nullptr) {}   // default constructor
Customer::Customer(const Customer& other): customerID(other.customerID), purchaseList(nullptr) {    // copy constructor 
    Node<Purchase>* curr = other.purchaseList;
    Node<Purchase>* last = nullptr;

    while (curr) {
        Node<Purchase>* newNode = new Node<Purchase>(curr->getData());
        if (!purchaseList) purchaseList = newNode;
        else if(last) last->setNext(newNode);

        last = newNode;
        curr = curr->getNext();
    }
}

Customer::~Customer() { // destructor
    while(purchaseList) {
        Node<Purchase>* temp = purchaseList;
        purchaseList = purchaseList->getNext();
        delete temp;
    }
}

// getters
string Customer::getCustomerID() const { return customerID; }
void Customer::setID(const string& id) { customerID = id; }
Node<Purchase>* Customer::getPurchaseList() const { return purchaseList; }

void Customer::addPurchase(const Purchase& p) {
    Node<Purchase>* newNode = new Node<Purchase>(p);    // create a new linked list
    
    if(!purchaseList) { // if the customer has no previous purchases
        purchaseList = newNode;
    } else  {   // otherwise, append to the end
        Node<Purchase>* curr = purchaseList;
        while(curr->getNext()) curr = curr->getNext();
        curr->setNext(newNode);
    }
}

json Customer::purchasesToJSON() const {
    json arr = json::array();
    Node<Purchase>* curr = purchaseList;
    while(curr) {
        arr.push_back(curr->getData().toJSON());    // convert each purchase to JSON
        curr = curr->getNext();
    }
    return arr;
}

json Customer::toJSON() const { // converts Customer obj into JSON obj
    return {
        {"customerID", customerID},
        {"purchases", purchasesToJSON()}
    };
}

Customer Customer::fromJSON(const json& j) {
    Customer c(j.at("customerID")); // create a customer object with the ID
    for(auto& p : j.at("purchases")) {  // iterate over each purchase in JSON
        Purchase pur = Purchase::fromJSON(p);
        c.addPurchase(pur);
    }
    return c;
}


// INVENTORY NODE class
InventoryNode::InventoryNode(Product* p, int q, int thresh)
        : product(p), quantity(q), threshold(thresh), left(nullptr), right(nullptr) {}  // parameterized constructor
    
// getters
Product* InventoryNode::getProduct() { return product; }
int InventoryNode::getQuantity() { return quantity; }
int InventoryNode::getThreshold() { return threshold; }
InventoryNode* InventoryNode::getLeft() { return left; }
InventoryNode* InventoryNode::getRight() { return right; }

// setters
void InventoryNode::setLeft(InventoryNode* l) { left = l; }
void InventoryNode::setRight(InventoryNode* r) { right = r; }
void InventoryNode::setQuantity(int q) { quantity = q; }
void InventoryNode::setThreshold(int t) { threshold = t; }

bool InventoryNode::needsRestock() { return quantity < threshold; }
void InventoryNode::displayProductDetails() {
    if(product) {
        cout << "Product ID: " << product->getProductID()
             << ", Price: " << product->getProductPrice()
             << ", Quantity: " << quantity
             << ", Threshold: " << threshold
             << (needsRestock() ? " (LOW STOCK!)" : "") << endl;
    }
}

InventoryNode::~InventoryNode() {   // destructor
    if(product) {
        delete product;
        product = nullptr;
    }
}

// FRANCHISE INVENTORY Class
FranchiseInventory::FranchiseInventory() : root(nullptr) {} // default constructor

void FranchiseInventory::addProduct(Product* p, int quantity, int threshold) {  // inserts a new product into the inventory BST
    InventoryNode* newNode = new InventoryNode(p, quantity, threshold);
        
    if(!root) { // if tree is empty
        root = newNode;
        return;
    }

    // traverses the BST to find the correct position for insertion
    InventoryNode* curr = root;
    InventoryNode* parent = nullptr;

    while(curr) {
        parent = curr;
        if(p->getProductID() < curr->getProduct()->getProductID()) {
            curr = curr->getLeft();
        } else {
            curr = curr->getRight();
        }
    }

    if(p->getProductID() < parent->getProduct()->getProductID()) {
        parent->setLeft(newNode);
    } else {
        parent->setRight(newNode);
    }
}

// recursive in-order traversal of BST (ensures products are displayed in sorted order by productID)
void FranchiseInventory::displayInventoryHelper(InventoryNode* node) {
    if(!node) {
        return;
    }
    displayInventoryHelper(node->getLeft());
    node->displayProductDetails();
    displayInventoryHelper(node->getRight());
}

// display all products
void FranchiseInventory::displayInventory() {
    if(!root) {
        cout << "Inventory is empty!\n";
        return;
    }
    cout << "INVENTORY\n";
    displayInventoryHelper(root);
}

// searches for a product in BST by productID
InventoryNode* FranchiseInventory::search(const string& productID) {
    InventoryNode* curr = root;
    while(curr) {   
        if(productID == curr->getProduct()->getProductID()) {
            return curr;
        } else if(productID < curr->getProduct()->getProductID()) {
            curr = curr->getLeft();
        } else {
            curr = curr->getRight();
        }
    }
    return nullptr;
}

// updates stock quantity when products are sold
void FranchiseInventory::updateStock(const string& id, int soldQuantity) {
    InventoryNode* node = search(id);
    if(!node) {
        cout << "Product not found.\n";
        return;
    }

    // checks for insufficient quantity and adjusts quantity if needed
    int available = node->getQuantity();
    if(soldQuantity > available) {
        cout << "Insufficient stock! Selling only available quantity: " << available << endl;
        soldQuantity = available;
    }

    node->setQuantity(available - soldQuantity);
    Product* p = node->getProduct();
    p->setProductSales(p->getProductSales()+soldQuantity);  // updates the product's total sales
}

// recursively checks each node for low stock
void FranchiseInventory::checkLowStockHelper(InventoryNode* node, bool& found) {
    if(!node) {
        return;
    }
    checkLowStockHelper(node->getLeft(), found);
    if(node->needsRestock()) {
        cout << "Product " << node->getProduct()->getProductID() << " is low on stock.\n";
        found = true;
    }

    checkLowStockHelper(node->getRight(), found);   // returns productID if found
    
}

// wrapper for low stock checking
void FranchiseInventory::checkLowStock() {
    if(!root) {
        cout << "Inventory is empty.\n";
        return;
    }

    bool found = false;
    cout << "Checking low stock items: ";
    checkLowStockHelper(root, found);

    if(!found) {
        cout << "No products in low stock!\n";
    }
}

// recursively traverses BST in-order
void FranchiseInventory::helperFunction(InventoryNode* node, json& arr) const {
    if(!node) return;
    helperFunction(node->getLeft(), arr);
    arr.push_back({ // adds each product's details to a JSON arr
        {"productID", node->getProduct()->getProductID()},
        {"price", node->getProduct()->getProductPrice()},
        {"totalQuantity", node->getQuantity()+node->getProduct()->getProductSales()},
        {"sales", node->getProduct()->getProductSales()},
        {"quantity", node->getQuantity()},
        {"threshold", node->getThreshold()}
    });
    helperFunction(node->getRight(), arr);
}

json FranchiseInventory::toJSON() const {
    json arr = json::array();
    helperFunction(root, arr);  
    return arr;
}

void FranchiseInventory::fromJSON(const json& j) {
    for(auto& item : j) {   // creates Product objs and inserts into BST
        Product* p = new Product(item.at("productID"), item.at("sales"), item.at("price"));
        addProduct(p, item.at("quantity"), item.at("threshold"));
    }
}

// converts BST to list for sorting
void FranchiseInventory::buildListFromBST(InventoryNode* node, SortNode*& head) {
    if(!node) {
        return;
    }

    buildListFromBST(node->getLeft(), head);

    // inserting at head (we'll sort later anywayss)
    SortNode* newNode = new SortNode(node->getProduct(), node->getQuantity(), node->getThreshold());
    newNode->next = head;
    head = newNode;

    buildListFromBST(node->getRight(), head);
}

FranchiseInventory::SortNode* 
FranchiseInventory::sortByPrice(SortNode* head) {
    SortNode* sorted = nullptr;

    while(head) {
        SortNode* curr = head;
        head = head->next;

        // if list is empty or current node should be at head
        if(!sorted || curr->product->getProductPrice() < sorted->product->getProductPrice()) {
            curr->next = sorted;
            sorted = curr;
        } else {
            // find the right spot in the sorted list
            SortNode* temp = sorted;
            while(temp->next && temp->next->product->getProductPrice() < curr->product->getProductPrice()) {
                temp = temp->next;
            }
            curr->next = temp->next;
            temp->next = curr;
        }
    }

    return sorted;
}

// build a list from BST and display product prices
void FranchiseInventory::displaySortedProductsByPrice() {
    SortNode* list = nullptr;
    buildListFromBST(root, list);

    list = sortByPrice(list);
    cout << "\nProducts Sorted by Price:\n";
    while(list) {
        cout << list->product->getProductID()
             << " | Price: " << list->product->getProductPrice()
             << endl;
        list = list->next;
    }
}

FranchiseInventory::SortNode*
FranchiseInventory::sortByThreshold(SortNode* head) {
    const int MAX_THRESHOLD = 100;
    SortNode* buckets[MAX_THRESHOLD + 1] = {nullptr};   // arr of buckets

    // distribute nodes
    while(head) {
        SortNode* curr = head;
        head = head->next;

        int t = curr->threshold;    // get thres value
        curr->next = buckets[t];    // insert node at head of corresponding bucket
        buckets[t] = curr;
    }

    SortNode* sortedHead = nullptr; // head of final sorted list
    SortNode* tail = nullptr;

    // combine buckets to get sorted linked list
    for(int i = 0; i <= MAX_THRESHOLD; i++) {
        while(buckets[i]) {
            SortNode* node = buckets[i];
            buckets[i] = buckets[i]->next;

            if(!sortedHead) {   // first node in sorted list
                sortedHead = tail = node;
                tail->next = nullptr;
            } else {    // append to tail
                tail->next = node;
                tail = node;
                tail->next = nullptr;
            }
        }
    }

    return sortedHead;  
}

// build a list from BST and display product thresholds
void FranchiseInventory::displaySortedProductsByThreshold() {
    SortNode* list = nullptr;
    buildListFromBST(root, list);

    list = sortByThreshold(list);

    cout << "\nProducts Sorted by Threshold:\n";
    while(list) {
        cout << list->product->getProductID()
             << " | Threshold: " << list->threshold
             << endl;

        list = list->next;
    }
}

// splits the list into 2 halves
FranchiseInventory::SortNode*
FranchiseInventory::splitList(SortNode* head)  {
    SortNode* slow = head;
    SortNode* fast = head->next;

    while(fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    SortNode* mid = slow->next; // mid is start of second half
    slow->next = nullptr;   // break the list
    return mid;
}

FranchiseInventory::SortNode*
FranchiseInventory::mergeBySales(SortNode* a, SortNode* b) {
    if(!a) return b;    // if one list is empty, return the other
    if(!b) return a;

    // pick smaller sales node and recurse
    if(a->product->getProductSales() <= b->product->getProductSales()) {
        a->next = mergeBySales(a->next, b);
        return a;
    } else {
        b->next = mergeBySales(a, b->next);
        return b;
    }
}

// performs merge sort on linked list based on product sales
FranchiseInventory::SortNode*
FranchiseInventory::mergeSortBySales(SortNode* head) {
    if(!head || !head->next)    // base case (empty or single node)
        return head;

    SortNode* mid = splitList(head);    // split list

    SortNode* left = mergeSortBySales(head);    // sort left half
    SortNode* right = mergeSortBySales(mid);    // sort right half

    return mergeBySales(left, right);
}

// build a list from BST and display product sales
void FranchiseInventory::displaySortedProductsBySales() {
    SortNode* list = nullptr;
    buildListFromBST(root, list); 

    list = mergeSortBySales(list);

    cout << "\nProducts Sorted by Sales (Merge Sort):\n";
    while(list) {
        cout << list->product->getProductID()
             << " | Sales: " << list->product->getProductSales()
             << " | Quantity: " << list->quantity
             << endl;
        list = list->next;
    }
}

void FranchiseInventory::deleteBST(InventoryNode* node) {
    if(!node) {
        return;
    }

    deleteBST(node->getLeft());
    deleteBST(node->getRight());
    delete node;
}

FranchiseInventory::~FranchiseInventory() {
    deleteBST(root);
    root = nullptr;
}

// FRANCHISE (MAIN) CLASS

void Franchise::addProduct(Product* p, int quantity, int threshold) {   // add product in inventory
    inv.addProduct(p, quantity, threshold);
}

void Franchise::activate() { isActive = true; }
void Franchise::deactivate() { isActive = false; }

// getters
bool Franchise::getStatus() { return isActive; }
FranchiseInventory& Franchise::getInventory() { return inv; }

void Franchise::showInventory() { 
    inv.displayInventory();
}

// sells a product by reducing its stock in the inv
void Franchise::sellProduct(const string& productID, int quantity) {
    InventoryNode* node = inv.search(productID);
    if(!node) {
        cout << "Product not found.\n";
        return;
    }

    // if requested quantity exceeds available stock, it sells only what's available
    if(node->getQuantity() < quantity) {
        cout << "Insufficient Stock! Selling available quantity: " << node->getQuantity() << endl;
        quantity = node->getQuantity();
    }

    inv.updateStock(productID, quantity);   // update stock
    cout << "Sold " << quantity << " of " << productID << endl;
}

// find customer in the linked list
Customer* Franchise::getCustomerRecord(const string& customerID) {
    Node<Customer>* curr = customersHead;

    while(curr) {
        if(curr->getData().getCustomerID() == customerID) {
            return &(curr->getData());
        }
        curr = curr->getNext();
    }
    return nullptr;
}

// create a customer if not present
Customer* Franchise::addCustomerifNotPresent(const string& customerID) {
    Customer* c = getCustomerRecord(customerID);

    if(c) {
        return c;
    }

    Customer newCustomer(customerID);
    Node<Customer>* newNode = new Node<Customer>(newCustomer);

    newNode->setNext(customersHead);
    customersHead = newNode;

    return &(customersHead->getData());
}

// add customer ID to queue for service
void Franchise::enqueueCustomer(const string& customerID) {
    customerQueue.enqueue(customerID);
}

// removes next customer from queue and prints which customer was served
void Franchise::dequeueCustomer() {
    if(customerQueue.isEmpty()) {
        cout << "No customers in queue.\n";
        return;
    }

    string temp = customerQueue.dequeue();
    cout << "Customer " << temp << " served.\n";
}

// adds a product to current customer's bill
void Franchise::addItemToBill(Product* p, int quantity) {
    Product temp = *p;  // pushing a product's copy with qty info
    temp.setProductSales(quantity);
    billingStack.push(temp);

    // FOR GRAPH
    string newID = p->getProductID();    // adding product as a vertex
    purchaseGraph.addVertex(newID);

    Stack<Product> tempStk;
    while(!billingStack.isEmpty()) {    // iterate through stack to create edges
        Product existingPdt = billingStack.pop();
        tempStk.push(existingPdt);

        string existingID = existingPdt.getProductID();
        if(existingID != newID) {
            purchaseGraph.addEdge(newID, existingID);   // link current product with previously added items
        }
    }

    while(!tempStk.isEmpty()) { // restore billing stack
        billingStack.push(tempStk.pop());
    }
}

// displays products that are frequently bought together with the given product
void Franchise::showFrequentlyBoughtWith(const string& productID) {
    GraphNode<string>* vertex = purchaseGraph.findVertex(productID);
    
    if(!vertex) {
        cout << "Product not found in purchase graph.\n";
        return;
    }

    cout << "Products frequently bought with " << productID << ": \n";
    Node<string>* edge = vertex->edges;
    
    if(!edge) {
        cout << "No frequently bought together products.\n";
        return;
    }

    while(edge) {
        cout << " - " << edge->getData() << endl;
        edge = edge->getNext();
    }
}

// displays the entire purchase graph (all vertices and their edges)
void Franchise::displayPurchaseGraph() {
    cout << "\n=== Frequently Bought Together Products ===\n";
    purchaseGraph.display();   
    cout << "========================================\n";
}

// adds a purchase record to franchise history
void Franchise::addPurchaseRecord(const Purchase& p, const string& customerID) {
    Node<Purchase>* newNode = new Node<Purchase>(p);
    newNode->setNext(purchaseHistory);
    purchaseHistory = newNode;  // add to purchase history linked list

    Customer* c = addCustomerifNotPresent(customerID);
    c->addPurchase(p);  // add to customer purchase list

    for(Node<Product>* first = p.getItems(); first != nullptr; first = first->getNext()) {
        string id1 = first->getData().getProductID();
        purchaseGraph.addVertex(id1);

        for(Node<Product>* second = first->getNext(); second != nullptr; second = second->getNext()) {
            string id2 = second->getData().getProductID();
            purchaseGraph.addVertex(id2);
            purchaseGraph.addEdge(id1, id2);
            purchaseGraph.addEdge(id2, id1); // undirected graph
        }
    }
}

// converts all customer data (with purchases) into JSON format
json Franchise::customersToJSON() const {
    json arr = json::array();
        
    Node<Customer>* curr = customersHead;

    while(curr) {
        Customer c = curr->getData();
        json purchases = c.purchasesToJSON();
           
        arr.push_back({
            {"customerID", c.getCustomerID()},
            {"purchases", purchases}
        });

        curr = curr->getNext();
    }
    return {{"customers", arr}};
}

// loads customer data and purchase history from JSON
void Franchise::customersFromJSON(const json& j) {
    customersHead = nullptr;

    for(const auto& item : j.at("customers")) {
        string cusID = item.at("customerID");

        // create new customer 
        Customer c(cusID);
        Node<Customer>* newNode = new Node<Customer>(c);
        newNode->setNext(customersHead);
        customersHead = newNode;

        // load purchase list
        for(const auto& p : item.at("purchases")) {
            Purchase pur = Purchase::fromJSON(p);
            customersHead->getData().addPurchase(pur);
        }
    }
}

// saves customer JSON data to a file
void Franchise::saveCustomersToFile(const string& filename) {
    ofstream fout(filename);
    if(!fout.is_open()) {
        return;
    }

    fout << customersToJSON().dump(4);
    fout.close();
}

// loads customer JSON data from a file
void Franchise::loadCustomersFromFile(const string& filename) {
    ifstream fin(filename);
    if(!fin.is_open() || fin.peek() == ifstream::traits_type::eof()) {
        return;
    }

    json j;
    fin >> j;
    fin.close();
    customersFromJSON(j);
}

// converts franchise obj to JSON (inv + purchase graph included)
json Franchise::toJSON() const {
    return {
        {"franchiseID", franchiseID},
        {"location", location},
        {"isActive", isActive},
        {"inventory", inv.toJSON()},
        {"purchaseGraph", purchaseGraph.toJSON()}
    };
}

// load franchise data from JSON
void Franchise::fromJSON(const json& j) {
    franchiseID = j.at("franchiseID");
    location = j.at("location");
    isActive = j.at("isActive");
    inv.fromJSON(j.at("inventory"));
    
    if (j.contains("purchaseGraph")) {
        purchaseGraph.fromJSON(j.at("purchaseGraph"));
    }
    
    if (j.contains("customers")) {
        customersFromJSON(j);
    }
}

// save franchise object to a file
void Franchise::saveToFile(const string& filename) {
    ofstream fout(filename);
    fout << toJSON().dump(4); 
    fout.close();
}

// load franchise obj from a file
void Franchise::loadFromFile(const string& filename) {
    ifstream fin(filename);
    if(!fin.is_open() || fin.peek() == ifstream::traits_type::eof()) {
        return;
    }

    json j;
    fin >> j;
    fin.close();

    fromJSON(j);
}

// displays franchise info
void Franchise::displayFranchises() {
    cout << "Franchise ID: " << franchiseID 
         << ", Location: " << location
         << ", Status: " << (isActive ? "Active" : "Inactive") << endl;
}

// displays full purchase history of all customers
void Franchise::displayPurchaseHistory() {
    Node<Customer>* cNode = customersHead;

    while (cNode) {
        Customer& c = cNode->getData();
        cout << "Customer ID: " << c.getCustomerID() << endl;

        Node<Purchase>* pNode = c.getPurchaseList();
        while (pNode) {
            Purchase& p = pNode->getData();

            cout << "  Purchase ID: " << p.getPurchaseID()
                 << ", Date: " << p.getDate()
                 << ", Total Paid: " << p.getTotalPrice() << endl;

            // iterate products inside this purchase
            Node<Product>* itemNode = p.getItems();
            while (itemNode) {
                Product& prod = itemNode->getData();

                cout << "     Product: " << prod.getProductID()
                     << ", Quantity: " << prod.getProductSales()
                     << ", Price: " << prod.getProductPrice()
                     << endl;

                itemNode = itemNode->getNext();
            }

            pNode = pNode->getNext();
        }

        cNode = cNode->getNext();
        cout << "-----------------------------" << endl;
    }
}

Franchise::~Franchise() {
    // delete purchase history linked list
    while(purchaseHistory) {
        Node<Purchase>* temp = purchaseHistory;
        purchaseHistory = purchaseHistory->getNext();
        delete temp;
    }

    // delete customers linked list
    while(customersHead) {
        Node<Customer>* temp = customersHead;
        customersHead = customersHead->getNext();
        delete temp;
    }

    // clear purchase graph
    purchaseGraph.clear();
}