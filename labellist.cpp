#include "labellist.h"
#include <cassert>
#include <iostream>

// Complete - Do not alter
LabelList::MsgNode::MsgNode(const MsgType& msg)
    : msg_(msg)
{
    // Every message is part of the "all" (index 0) label
    labelled_.push_back(true);
    next_.push_back(nullptr);
    prev_.push_back(nullptr);
}

// Complete - Do not alter
LabelList::MsgToken::MsgToken()
 : node_(nullptr), list_(nullptr)
{ 
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken::MsgToken(MsgNode* node, LabelList* list)
// Add any initializers here
    : node_(node), list_(list)
{
}

// To be completed - must adhere to any requirements in the .h file
bool LabelList::MsgToken::operator==(const MsgToken& rhs) const
{
    // checks if two MsgTokens reference the same MsgNode
    
    if (node_ == rhs.node_){
        return true;
    }

    else {
        return false;
    }
}

// Complete - Do not alter
bool LabelList::MsgToken::operator!=(const MsgToken& rhs) const
{
    return !operator==(rhs);
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken::operator bool() const
{
    if (node_){ // checks if the pointer to msgNode is valid
        return true;
    }
    else{
        return false;
    }
}

// Complete - Do not alter
bool LabelList::MsgToken::isValid() const
{
    return this->node_ != nullptr;
}

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::next(const LabelType& label)
{
    // Add your code here
    size_t index = this->list_->findLabelIndex(label);
    if( false == this->isLabelMember(index) ) 
    {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->next_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::prev(const LabelType& label)
{
    // Add your code here
    size_t index = this->list_->findLabelIndex(label);
    if( false == this->isLabelMember(index) ) 
    {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->prev_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(const LabelType& label) const
{
    size_t index = this->list_->findLabelIndex(label);
    return isLabelMember(index);
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(size_t index) const
{
    return this->isValid() && 
            index != INVALID_LABEL &&
            index < this->node_->labelled_.size() &&
            this->node_->labelled_[index] ;
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgType const & LabelList::MsgToken::msg() const
{
    if (this->isValid() == false){
        throw std::logic_error("msg is not valid");
    }
    return node_-> msg_;
}
// To be completed - must adhere to any requirements in the .h file
LabelList::MsgType& LabelList::MsgToken::msg() 
{
    //note: returns the message of particular node token is pointing to
    if (this->isValid() == false){
        throw std::logic_error("msg is not valid");
    }
    return node_->msg_;
}

// To be completed - must adhere to any requirements in the .h file
std::ostream& operator<<(std::ostream& ostr, const LabelList::MsgToken& token)
{
    if(token.isValid())
    {
        ostr << token.node_->msg_;
    }
    return ostr;
}

// Static constant 
const LabelList::MsgToken LabelList::end_(nullptr, nullptr);
 
// To be completed - must adhere to any requirements in the .h file
LabelList::LabelList() //construct new label list object with "all" label
// Add any initializers here
:labels_(1,"all"),heads_(1, NULL), tail_(NULL,this)
{
    
}

// To be completed - must adhere to any requirements in the .h file
LabelList::~LabelList()
{
  clear();
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::clear()
{   
    
        //destroys all message nodes
        if (heads_[0]!=NULL){
            MsgNode* curr = heads_[0];
            MsgNode* nex = heads_[0];
            while (curr != NULL){
                nex = curr->next_[0];
                curr->next_[0] = NULL;
                curr->prev_[0] = NULL;
                delete curr;
                curr = nex;
                
                
            }
        }
        heads_[0] = NULL;
        //tails_[0] = NULL; 
        // size_t numLabels = labels_.size();  
        // for (size_t i = 0; i<numLabels - 2; i++){
        //     labels_.pop_back();
        //     tails_.pop_back();
        // }     
    
}

// To be completed - must adhere to any requirements in the .h file
bool LabelList::empty() const
{
    //Returns true if no messages are in the "all" list
    if (heads_[0] == NULL){
        return true;
    }
    else{
        return false;
    }
}

// Complete
size_t LabelList::size() const
{
    MsgNode* n = this->heads_[0];
    size_t cnt = 0;
    while(n != nullptr){
        cnt++;
        n = n->next_[0];
    }
    return cnt;
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken LabelList::add(const MsgType& msg)
{//add new message to "all list"

    //create new msgNode
    MsgNode* newNode = new MsgNode(msg);
  
    
    //if "all" label is empty
    if (heads_[0]== NULL){
        heads_[0]=newNode;
        tail_ = MsgToken(newNode,this) ;
        newNode->prev_[0] = NULL;
        newNode->next_[0] = NULL;
        newNode->labelled_[0] = true;
    }
    
    //if "all" label has existing msgNodes
    else{
        //reconnect pointer to new node 
        MsgNode* temp = tail_.node_;
        
        temp->next_[0] = newNode;
        newNode->prev_[0] = temp;
        newNode->next_[0] = NULL;
        tail_.node_ = newNode;
        newNode->labelled_[0] = true;
    }
    //return msgToken
    return MsgToken(newNode, this); 
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::remove(const MsgToken& token)
{
    //check if valid token
    if (token == true){

        for(size_t i = 0; i< labels_.size(); i++){

            //for each label: check if this node is in label
            if ((token.node_->labelled_[i]) == true){

                //set node[i] label to F
                token.node_->labelled_[i] = false;

                 //if removed node is the only node for that label
                if (token.node_->prev_[i] == NULL && token.node_->next_[i]==NULL){
                    heads_[i] = NULL;
                    //tail_.node_ = NULL;
                    
                    token.node_->prev_[i] = NULL;
                    token.node_->next_[i]= NULL;

                }
                //if removed node is last for that label
                else if (token.node_->next_[i]==NULL ){
                    token.node_->prev_[i]->next_[i] = NULL;
                    token.node_->prev_[i] = NULL;
                }

                //if removed node is the first msg node
                else if (token.node_->prev_[i]==NULL){
                    heads_[i] = token.node_->next_[i];
                    token.node_->next_[i]->prev_[i] = NULL;
                }

                //if removed node is in the middle of linked list
                else{
                    token.node_->prev_[i]->next_[i] = token.node_->next_[i];
                    (token.node_)->next_[i]->prev_[i] = token.node_->prev_[i];
                    token.node_->prev_[i] = NULL;
                    token.node_->next_[i]= NULL;
                }
            }
        }
        //delete MsgNode
        delete token.node_;
    }
    else{
        return; 
    }
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::label(const MsgToken& token, const LabelType& label)
{
    if(!token.isValid())
    {
        throw std::invalid_argument("Bad token in label()");
    }
    
    //if label doesn't exist
    if (findLabelIndex(label) == INVALID_LABEL){
        //add new label to labels_ vector
        labels_.push_back(label);

        //link start pointer to newly labeled node
        heads_.push_back(token.node_);
        //tails_.push_back(token.node_);
        token.node_->next_.resize(labels_.size(),NULL);
        token.node_->prev_.resize(labels_.size(),NULL);
        
        // std::cout << labels_.size() << std::endl;
        // std::cout << token.node_->next_.size() << std::endl;
        
        // set newly added row to false 
        size_t i = findLabelIndex(label);
        MsgNode* temp = heads_[0];
        while(temp != nullptr){
            temp->labelled_.push_back(false);
            temp = temp->next_[0];
        }
        token.node_->labelled_[i] = true;
        
    }
    else{

        MsgNode* n = token.node_;
        MsgNode* p = token.node_;
    
        // determine order of tokens
        size_t i = findLabelIndex(label);
        if (label == "all"){
            return;
        }
        // check if trying to remove from "all"
        
        while(n != nullptr){
            if(n->labelled_[i] == true ){
                break;
            }
            n = n->next_[0]; 
        }
        while(p != nullptr){
            if(p->labelled_[i] == true ){
                break;
            }
            p = p->prev_[0]; 
        }

        // if adding to an empty list
        if (p == nullptr && n == nullptr){
            token.node_->labelled_[i] = true;
            heads_[i] = token.node_;
            //tails_[i] = token.node_;
            token.node_->next_[i] = NULL;
            token.node_->prev_[i] = NULL;
        }
        // if labled token is in the middle of 2 existing tokens
        else if (p != nullptr && n != nullptr){
            token.node_->labelled_[i] = true;
            n->prev_[i] = token.node_;
            p->next_[i] = token.node_;

            token.node_->next_.resize(labels_.size(),NULL);
            token.node_->prev_.resize(labels_.size(),NULL);

            token.node_->prev_[i] = p;
            token.node_->next_[i] = n;
        }

        //if labeled token will be the new head
        else if (p == nullptr){
            token.node_->labelled_[i] = true;
            heads_[i]->prev_[i] = token.node_;

            token.node_->next_.resize(labels_.size(),NULL);
            token.node_->prev_.resize(labels_.size(),NULL);

            token.node_->prev_[i] = nullptr;
            token.node_->next_[i] = heads_[i];
            
            heads_[i] = token.node_;
        }
        //if labeled token already exists and is after previously labeled
        else if (n == nullptr){
            size_t i = findLabelIndex(label);
            token.node_->labelled_[i] = true;
            //tails_[i]->next_[i] = token.node_;
            p ->next_[i] = token.node_;
        
            token.node_->next_.resize(labels_.size(),NULL);
            token.node_->prev_.resize(labels_.size(),NULL);

            token.node_->prev_[i] = p;
            token.node_->next_[i] = nullptr;

            // std::cout << labels_.size() << std::endl;
            // std::cout << token.node_->next_.size() << std::endl;
            //tails_[i] = token.node_;
            //std::cout<<token.node_->msg_;
        }
    }
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::unlabel(const MsgToken& token, const LabelType& label)
{/*Removes the referenced message from a given label and its list. 
     *        Attempting to remove a message from the "all" label should have
     *        no effect.
     */ 
    if(!token.isValid())
    {
        throw std::invalid_argument("Bad token in label()");
    }
    //if trying to unlabel "all"
    if (label == "all"){
        return;
    }
    //get index of the label
    size_t i = getLabelIndex(label);

    //if unlabel a msg not part of that label
    if (token.isLabelMember(i) == false){
        return;
    }

    //use token to set labelled_ to false
    token.node_->labelled_[i] = false;

    // if token is the only message in linked list
    if (token.node_->prev_[i] == NULL && token.node_->next_[i] == NULL ){
        heads_[i] = NULL;
        //tails_[i] = NULL;
    }
    //check if token is first, but not last message in linked list 
    else if (token.node_->prev_[i] == NULL ){
        heads_[i] = token.node_->next_[i];
        token.node_->next_[i]->prev_[i] = NULL;
    } 
    //if token is last message in linked list
    else if (token.node_->next_[i] == NULL){
        token.node_->prev_[i]->next_[i] = NULL;

         
    }
    //else if in the middle:
    else{

        //reset pointers of message before and after
        token.node_->prev_[i]->next_[i] = token.node_->next_[i];
        token.node_->next_[i]->prev_[i] = token.node_->prev_[i];
         // reset the pointers of unlabeled message linked list
        token.node_->next_[i] = NULL;
        token.node_->prev_[i] = NULL;
    }
}

// Complete - Do not alter
LabelList::MsgToken LabelList::find(const MsgType& msg, const LabelType& label)
{
    // See if the label exists, will throw if not
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while(n != nullptr)
    {
        if(n->msg_ == msg)
        {
            MsgToken temp = MsgToken(n,this);
            return temp;
            break;
        }
        n = n->next_[level]; 
    }
    return end();
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken LabelList::find(size_t index, const LabelType& label)
{//returns a token to the message whose position/index matches the specific index 
//note: index is the index of message in given label's list

    //check if index is valid 
    size_t level = this->getLabelIndex(label);
    MsgNode* temp = this->heads_[level];
    for (size_t i = 0; i < index; i++){
        //traverse until index
        temp = temp->next_[level];
        if (temp == NULL){
            return end();
        }
    }

    //return msgToken
    return MsgToken(temp, this); // does this need to be dynamically allocated?  
}

// Complete - Do not alter
LabelList::MsgToken const & LabelList::end() const
{
    return end_;
}

// Complete - Do not alter
void LabelList::print(std::ostream& ostr, const LabelType& label, char separator) const
{
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while(nullptr != n)
    {
        ostr << n->msg_ << separator;
        n = n->next_[level];
    }
    ostr << std::endl;
}

// To be completed - must adhere to any requirements in the .h file
size_t LabelList::findLabelIndex(const LabelType& label) const
{
    for (size_t i = 0; i< labels_.size(); i++){
        if (labels_[i]== label){
            return i;
        }
    }
    return INVALID_LABEL;
}

// Complete - Do not alter
size_t LabelList::getLabelIndex(const LabelType& label) const
{
    size_t retval = this->findLabelIndex(label);
    if(INVALID_LABEL == retval)
    {
        throw std::out_of_range("Label doesn't exist");
    }
    return retval;
}


