#include <iostream>
#include <fstream>
#include <string>

/* @Gülsen Eryigit
Student Name: <Muhammet ASLAN> 
Student ID: <150160031> 
Date: <21.12.2019> */

using namespace std;

class Node{
public:
    string type;
    int id;
    Node *parent;
    Node **majors;
    int major_count;
    Node **minors;
    int minor_count;
    Node(string type, int id){
        this->type = type;
        this->id = id;
        major_count = 0;
        minor_count = 0;
        parent = NULL;
        majors = new Node*[0];
        minors = new Node*[0];
    }
};

int increment_list(Node ** &list, int size){
    Node **tmp = new Node*[size];
    for(int i = 0; i < size; i++){
        tmp[i] = list[i];
    }
    delete[] list;
    list = new Node*[size+1];
    for(int i = 0; i < size; i++){
        list[i] = tmp[i];
    }
    return size+1;
}

class Network{
public:
    Node *base;
    void create_base();
    void add_element(Node *, string &, int, int);
    void prefix_print(Node *);
};

void Network::create_base(){
    base = new Node("BS", 0);
}

void Network::add_element(Node *sudo_base, string &type, int id, int parent_id){
    if( sudo_base ){
        if( sudo_base->id == parent_id ){
            Node *new_member = new Node(type, id);
            new_member->parent = sudo_base;
            if( type.compare("BS") == 0 ){
                sudo_base->major_count = increment_list(sudo_base->majors, sudo_base->major_count);
                sudo_base->majors[sudo_base->major_count-1] = new_member;
            }
            else if( type.compare("MH") == 0 ){
                sudo_base->minor_count = increment_list(sudo_base->minors, sudo_base->minor_count);
                sudo_base->minors[sudo_base->minor_count-1] = new_member;
            }
        }
        else{
            for(int i = 0; i < sudo_base->major_count; i++){
                add_element(sudo_base->majors[i], type, id, parent_id);
            }
        }
    }
}

void Network::prefix_print(Node *base){
    if( !base ) return;
    cout << base->type << " " << base->id << endl;
    for(int i = 0; i < base->major_count; i++)
        prefix_print(base->majors[i]);
}

Node * find(Node *root, int id){
    if( root ){
        if(root->id) cout << " " << root->id;
        Node *mh = NULL;
        for(int i = 0; i < root->minor_count; i++){
            if( root->minors[i]->id == id ){
                return root->minors[i];
            }
        }
        for(int i = 0; i < root->major_count; i++){
            mh = find(root->majors[i], id);
            if( mh ) break;
        }
        return mh;
    }
    else return NULL;
}

void path(Node *mh){
    if(mh && mh->id != 0){
        path(mh->parent);
        cout << " " << mh->id;
    }
}

int main(int argc, char **argv){

    string type;
    int id;
    int parent_id;

    string buffer;
    string message;
    int dest_id;

    Network network;
    network.create_base();

    ifstream f1(argv[1]);
    while(f1 >> type >> id >> parent_id){
        network.add_element(network.base, type, id, parent_id);
    }
    f1.close();

    ifstream f2(argv[2]);
    while(getline(f2, buffer) && int(buffer.length()) > 1){
        int split_pos = buffer.length();
        for(int i = 0; i < int(buffer.length()); i++){
            if( buffer[i] == '>' ){
                split_pos = i;
                break;
            }
        }
        message = buffer.substr(0, split_pos);
        dest_id = stoi( buffer.substr(split_pos+1, buffer.length()) );
        cout << "Traversing:0";
        Node *mh = find(network.base, dest_id);
        cout << endl;
        if(mh){
            cout << "Message:" << message << " To:0";
            path(mh->parent);
            cout << " mh_" << dest_id << endl;
        }
        else{
            cout << "Can not be reached the mobile host mh_" << dest_id << " at the moment" << endl;
        }

    }
    f2.close();

    return 0;
}
