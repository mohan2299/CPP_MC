#include<bits/stdc++.h>
using namespace std;

int minFun(int a, int b){
    if(a<b){
        return a;
    }
    else{
        return b;
    }
}

class Commission{

    public:

    int percentage;
    int maxCap;

};

class Payouts{

    public:

    vector<pair<string, int> > payoutHistory;
};


class Affiliate{

    public:

    int affiliateId;
    vector<pair<string, int> > orderComm;
    vector<pair<string, int> > orderPayout;
    vector<pair<string, int> > orderHistory;
    vector<pair<int, int> > payoutsMade;
    map<int, Payouts > mp;

};



class Order{

    public:

    string orderId;
    int price;
    string productId;
    string categoryId;
    int affiliateId;
    string timestamp;
    string state;

};


Order processOrder(string oId, int prc, string pId, string category, int affiliateId, string time, string st, 
                        unordered_map<string, Order> &orderMap, unordered_map<string, Commission> &commissionMap, unordered_map<int,Affiliate> &affiliateMap){
        
    Order o;
    o.orderId=oId;
    o.price=prc;
    o.productId=pId;
    o.categoryId=category;
    o.affiliateId=affiliateId;
    o.timestamp=time;
    o.state=st;

    if(st=="CREATED"){

        int comm=0;
        if(commissionMap[o.categoryId].percentage==-1){
            comm=commissionMap[o.categoryId].maxCap;
        }
        else{
            int a = commissionMap[o.categoryId].maxCap;
            int b = (commissionMap[o.categoryId].percentage * o.price) / 100 ;
            comm=minFun(a,b);
        }
        if(affiliateMap.find(o.affiliateId)==affiliateMap.end()){

            Affiliate a;
            a.affiliateId=o.affiliateId;

            affiliateMap[o.affiliateId]=a;
        }


        pair<string, int> p;
        p.first=o.orderId;
        p.second=comm;
        affiliateMap[o.affiliateId].orderComm.push_back(p);

        p.second=0;
        affiliateMap[o.affiliateId].orderPayout.push_back(p);
    }

    return o;
        
}

void processOrder(string oId, string state, unordered_map<string, Order> &ordersMap,
                    unordered_map<int,Affiliate> &affiliateMap){
        
    if(state=="CANCELLED"){

        if(ordersMap[oId].state=="CREATED" || ordersMap[oId].state=="DISPATCHED"){
            ordersMap[oId].state=state;
            int affId=ordersMap[oId].affiliateId;

            for(int i=0;i<affiliateMap[affId].orderComm.size();i++){
                if(affiliateMap[affId].orderComm[i].first==oId){
                    affiliateMap[affId].orderComm.erase(affiliateMap[affId].orderComm.begin() + i);
                    affiliateMap[affId].orderPayout.erase(affiliateMap[affId].orderPayout.begin() + i);
                    break;
                }
            }
            cout<<"Order is Cancelled !"<<endl;
        }
        else{
            cout<<"Invalid Operation !"<<endl;
        }
    }

    if(state=="RETURNED"){

        if(ordersMap[oId].state=="DELIVERED"){
            ordersMap[oId].state=state;
            int affId=ordersMap[oId].affiliateId;

            for(int i=0;i<affiliateMap[affId].orderComm.size();i++){
                if(affiliateMap[affId].orderComm[i].first==oId){
                    affiliateMap[affId].orderComm.erase(affiliateMap[affId].orderComm.begin() + i);
                    affiliateMap[affId].orderPayout.erase(affiliateMap[affId].orderPayout.begin() + i);
                    break;
                }
            }
            cout<<"Order is Returned !"<<endl;
        }
        else{
            cout<<"Invalid Operation !"<<endl;
        }
    }

    if(state=="RETURN_PERIOD_EXPIRED"){
        
        if(ordersMap[oId].state=="DELIVERED"){
            int affId=ordersMap[oId].affiliateId;
            ordersMap[oId].state=state;

            for(int i=0;i<affiliateMap[affId].orderComm.size();i++){
                if(affiliateMap[affId].orderComm[i].first==oId){
                    affiliateMap[affId].orderPayout[i].second=affiliateMap[affId].orderComm[i].second;
                    pair<string, int> p;
                    p.first=affiliateMap[affId].orderPayout[i].first;
                    p.second=affiliateMap[affId].orderPayout[i].second;
                    affiliateMap[affId].orderHistory.push_back(p);
                    break;
                }
            }
        }
        else{
            cout<<"Invalid Operation !"<<endl;
        }
        
    }

    if(state=="DISPATCHED"){

        if(ordersMap[oId].state=="CREATED"){
            ordersMap[oId].state=state;
            cout<<"Your order is "<<state<<" !"<<endl;
        }
        else{
            cout<<"Invalid Operation !"<<endl;
        }
        
    }


    if(state=="DELIVERED"){

         if(ordersMap[oId].state=="DISPATCHED"){
            ordersMap[oId].state=state;
            cout<<"Your order is "<<state<<" !"<<endl;
        }
        else{
            cout<<"Invalid Operation !"<<endl;
        }
    }

}


void payOut(int affId, unordered_map<int, Affiliate> &affiliateMap){

    unordered_map<int, Affiliate>:: iterator itr;
    int comm=0;

    for(itr=affiliateMap.begin();itr!=affiliateMap.end();itr++){
        
        if(itr->first==affId){
            for(int i=0;i<itr->second.orderPayout.size();i++){
                comm+=itr->second.orderPayout[i].second;
            }
            break;
        }
    }
    if(comm>=100){

        Payouts objP;

        for(int i=0;i<itr->second.orderPayout.size();i++){
            pair<string, int> p;
            p.first=itr->second.orderPayout[i].first;
            p.second=itr->second.orderPayout[i].second;
            objP.payoutHistory.push_back(p);
        }
        itr->second.orderPayout.clear();
        itr->second.orderComm.clear();

        int idx=itr->second.payoutsMade.size();
        itr->second.mp[idx+1]=objP;

        pair<int, int > p;
        p.first=idx+1;
        p.second=comm;
        itr->second.payoutsMade.push_back(p);

        cout<<"You will get Rs."<<comm<<" as payout."<<endl;
    }
    else{
        cout<<"Minimum payout amount not reached. Your payout balance is: "<<comm<<endl;
    }
}


void fetchTransaction(int affId, unordered_map<int, Affiliate> &affiliateMap){

    unordered_map<int, Affiliate>:: iterator itr;

    for(itr=affiliateMap.begin();itr!=affiliateMap.end();itr++){
        
        if(itr->second.affiliateId == affId){

            for(int i=0; i<itr->second.payoutsMade.size(); i++){
                int id=itr->second.payoutsMade[i].first;
                cout<<"TransactionID : "<<id<<endl;
                cout<<"Amount : "<<itr->second.payoutsMade[i].second<<endl;

                for(int j=0; j<itr->second.mp[id].payoutHistory.size(); j++ ){
                    cout<<" OrderID: "<<itr->second.mp[id].payoutHistory[j].first<<endl;
                    cout<<" OrderID: "<<itr->second.mp[id].payoutHistory[j].second<<endl;
                }
                cout<<endl;
            }
            break;
        }
    }

    if(itr==affiliateMap.end()){
        cout<<"Transaction hasn't been done yet."<<endl;
    }
}


int main(){

    unordered_map<string, Commission> commissionMap;
    commissionMap["Clothing"].percentage=-1;
    commissionMap["Clothing"].maxCap=150;

    commissionMap["Mobile"].percentage=10;
    commissionMap["Mobile"].maxCap=500;

    commissionMap["MobileCover"].percentage=-1;
    commissionMap["MobileCover"].maxCap=50;

    commissionMap["Furniture"].percentage=15;
    commissionMap["Furniture"].maxCap=1500;


    unordered_map<string, Order> ordersMap;
    unordered_map<int, Affiliate> affiliateMap;


    ordersMap["order1"]=processOrder("order1",600,"pid1","Mobile",1,"6 Apr 2024 18:00","CREATED",ordersMap,commissionMap,affiliateMap);
    //processOrder("order1","CANCELLED", ordersMap, affiliateMap);
    processOrder("order1","DISPATCHED", ordersMap, affiliateMap);
    processOrder("order1","DELIVERED", ordersMap, affiliateMap);
    processOrder("order1","RETURN_PERIOD_EXPIRED", ordersMap, affiliateMap);
    cout<<endl;

    ordersMap["order2"]=processOrder("order2",1000,"pid2","Clothing",1,"6 Apr 2024 18:00","CREATED",ordersMap,commissionMap,affiliateMap);
    //processOrder("order1","CANCELLED", ordersMap, affiliateMap);
    processOrder("order2","DISPATCHED", ordersMap, affiliateMap);
    processOrder("order2","DELIVERED", ordersMap, affiliateMap);
    processOrder("order2","RETURNED", ordersMap, affiliateMap);
    processOrder("order2","RETURN_PERIOD_EXPIRED", ordersMap, affiliateMap);
    payOut(1,affiliateMap);
    cout<<endl;

    fetchTransaction(1,affiliateMap);
    cout<<endl;


    ordersMap["order3"]=processOrder("order3",1000,"pid3","Furniture",1,"6 Apr 2024 18:00","CREATED",ordersMap,commissionMap,affiliateMap);
    //processOrder("order1","CANCELLED", ordersMap, affiliateMap);
    processOrder("order3","DISPATCHED", ordersMap, affiliateMap);
    processOrder("order3","DELIVERED", ordersMap, affiliateMap);
    processOrder("order3","RETURN_PERIOD_EXPIRED", ordersMap, affiliateMap);
    payOut(1,affiliateMap);
    cout<<endl;
    
    ordersMap["order4"]=processOrder("order4",200,"pid4","MobileCover",1,"6 Apr 2024 18:00","CREATED",ordersMap,commissionMap,affiliateMap);
    //processOrder("order1","CANCELLED", ordersMap, affiliateMap);
    processOrder("order4","DISPATCHED", ordersMap, affiliateMap);
    processOrder("order4","DELIVERED", ordersMap, affiliateMap);
    processOrder("order4","RETURN_PERIOD_EXPIRED", ordersMap, affiliateMap);
    payOut(1,affiliateMap);
    cout<<endl;

    fetchTransaction(1,affiliateMap);
    cout<<endl;

    //ordersMap["order1"]=processOrder("order1",600,"pid1","mobile",1,"6 Apr 2024 18:00","CREATED",ordersMap,commissionMap,affiliateMap);
}