#include<bits/stdc++.h>
#include <string> 
#include <vector>
using namespace std;

class Restaurant{

    public:

        string name;
        float rating;
        vector<pair<string,int> > menu;
        int maxOrders;
        int currOrders;
};

class Order{
	public:
		
		string cust_name;
		vector<pair<string,int> > orderedItems;
		int cost;
        float rating;
		string rest_name;
        string selection;
};

vector<pair<string,int> > add_menu(vector<pair<string,int> > items){
    vector<pair<string,int> > ls;
    for(int i=0;i<items.size();i++){
        pair<string, int> p;
        p.first=items[i].first;
        p.second=items[i].second;
        ls.push_back(p);
    }
    return ls;
}

Restaurant onboardRestaurant(string nm, int maxOrd, float rt, vector<pair<string, int> > items){

    Restaurant rest;
    rest.name=nm;
    rest.rating=rt;
    rest.maxOrders=maxOrd;
    vector<pair<string,int> > a = add_menu(items);
	rest.menu = a;
    rest.currOrders=0;

    return rest;
}

void updateMenu(Restaurant r, string updateType, string item, int price){
    if(updateType=="add"){
        r.menu.push_back(make_pair(item,price));
    }
    else{
        for(int i=0;i<r.menu.size();i++){
            if(r.menu[i].first=="item"){
                r.menu[i].second=price;
                break;   
            }
        }
    }
}

vector<pair<string,int> > add_order(vector<pair<string,int> > orderItems){
    vector<pair<string,int> > ls;
    for(int i=0;i<orderItems.size();i++){
        pair<string, int> p;
        p.first=orderItems[i].first;
        p.second=orderItems[i].second;
        ls.push_back(p);
    }
    return ls;
}

bool placeOrder(string orderName,string user, vector<pair<string,int> > orderItems, string selectionCriteria, 
                unordered_map<string,Restaurant> &restMap, unordered_map<string,Order> &ordersMap){

    Order cust;

    cust.cust_name=user;
    cust.selection=selectionCriteria;
    vector<pair<string,int> > a = add_order(orderItems);
    cust.orderedItems=a;
    cust.cost=INT_MAX;
    cust.rating=0.0;
    cust.rest_name="";

    string restaurant;

    unordered_map<string,Restaurant>:: iterator itr;
    string restName="";
    for(itr=restMap.begin(); itr != restMap.end(); itr++){
        int flag=0;
        int price=0;
        
        if(itr->second.currOrders < itr->second.maxOrders){
            
            for(auto pair : a){
                string item = pair.first;
                int qty = pair.second;
                int i;
                for(i=0;i<itr->second.menu.size();i++){
                    if(itr->second.menu[i].first==item){
                        break;
                    }
                }
                if(i<itr->second.menu.size()){
                    price+=qty*itr->second.menu[i].second;
                    flag=1;
                }
                else{
                    price=0;
                    flag=0;
                    break;
                }
            }
        }
        if(flag==1){
            if(selectionCriteria=="LowestCost"){
                if(cust.cost>price){
                    cust.cost=price;
                    cust.rating=itr->second.rating;
                    cust.rest_name=itr->first;
                    restName=itr->first;
                }
            }
            else{
                if(itr->second.rating>cust.rating){
                    cust.cost=price;
                    cust.rating=itr->second.rating;
                    cust.rest_name=itr->first;
                    restName=itr->first;
                }
            }
        }
    }

    ordersMap[orderName]=cust;

    if(restName==""){
        return false;
    }
    else{
        restMap[restName].currOrders += 1;
        return true;
    }

}

void updateOrder(string restName, string orderName, string Status,unordered_map<string, Restaurant> &restMap, unordered_map<string,Order> &ordersMap){

    if(Status == "Completed"){
        if(restMap[restName].currOrders>0){
            restMap[restName].currOrders--;
        }
        ordersMap.erase(orderName);

        cout<<orderName<<" is completed by "<<restName<<" !"<<endl;;
    }

}


int main(){

    unordered_map<string,Restaurant> restMap;
    unordered_map<string,Order> ordersMap;


    vector<pair<string,int> > items1;
    items1.push_back(make_pair("VegBiriyani",100));
    items1.push_back(make_pair("ChickenBiryani",150));

    vector<pair<string,int> > items2;
    items2.push_back(make_pair("VegBiriyani", 80));
    items2.push_back(make_pair("Idly", 10));
    items2.push_back(make_pair("Dosa", 50));
    items2.push_back(make_pair("ChickenBiriyani", 175));

    vector<pair<string,int> > items3;
    items3.push_back(make_pair("GobiManchurian", 150));
    items3.push_back(make_pair("Idly", 15));
    items3.push_back(make_pair("Dosa",30));
    items3.push_back(make_pair("ChickenBiriyani", 175));

    restMap["resta1"] = onboardRestaurant("resta1", 5, 4.5, items1);
	restMap["resta2"] = onboardRestaurant("resta2", 5, 4, items2);
    restMap["resta3"] = onboardRestaurant("resta3", 1, 4.9, items3);


    vector<pair<string,int> > orders1;
    orders1.push_back(make_pair("Idly",3));
    orders1.push_back(make_pair("Dosa",1));

    vector<pair<string,int> > orders2;
    orders2.push_back(make_pair("VegBiriyani",3));

    if(placeOrder("Order1","Ashwin", orders1, "LowestCost", restMap, ordersMap)){
        cout<<"Order Placed Successfully in "<<ordersMap["Order1"].rest_name<<endl;
    }
    else{
        cout<<"Order Cannot be Placed. Try again later !!"<<endl;
    }
    
    if(placeOrder("Order2","Harish", orders1, "LowestCost", restMap, ordersMap)){
        cout<<"Order Placed Successfully in "<<ordersMap["Order2"].rest_name<<endl;
    }
    else{
        cout<<"Order Cannot be Placed. Try again later !!"<<endl;
    }

    if(placeOrder("Order3","Shruti", orders2,"HighestRating", restMap, ordersMap)){
        cout<<"Order Placed Successfully in "<<ordersMap["Order3"].rest_name<<endl;
    }
    else{
        cout<<"Order Cannot be Placed. Try again later !!"<<endl;
    }

    updateOrder("resta3","Order1","Completed",restMap,ordersMap);


    if(placeOrder("Order4","Harish", orders1,"LowestCost", restMap, ordersMap)){
        cout<<"Order Placed Successfully in "<<ordersMap["Order4"].rest_name<<endl;
    }
    else{
        cout<<"Order Cannot be Placed. Try again later !!"<<endl;
    }


    vector<pair<string,int> > orders3;
    orders2.push_back(make_pair("PaneerTikka",1));
    orders2.push_back(make_pair("Idly",1));

    if(placeOrder("Order5","xyz", orders3,"LowestCost", restMap, ordersMap)){
        cout<<"Order Placed Successfully in "<<ordersMap["Order5"].rest_name<<endl;
    }
    else{
        cout<<"Order Cannot be Placed. Try again later !!"<<endl;
    }

    return 0;
}