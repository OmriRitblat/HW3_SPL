package bgu.spl.net.srv;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionImp<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler> clients; //<connction id, connection handler>
    private ConcurrentHashMap<String, List<Integer>> subsribtion; //<channel name, list<connection id>>

    public boolean send(int connectionId, T msg){
        ConnectionHandler c=clients.get(connectionId);
        if(c!=null) {
            c.send(msg);
            return true;
        }
        return false;
    }

    public void send(String channel, T msg){
        List<Integer> sub=subsribtion.get(channel);
        if(sub!=null)
            for(Integer i:sub)
                send(i,msg);
    }

    public void disconnect(int connectionId){
        clients.remove(connectionId);
        //should remove from subscibtion also?
    }

    public void addConnect(int connectionId,ConnectionHandler ch){
        clients.put(connectionId,ch);
    }

    public void addSubscribtion(String channel,int subId){
        if(!subsribtion.containsKey(channel))
            subsribtion.put(channel,new ArrayList<>());
        subsribtion.get(channel).add(subId);
    }

    public boolean removeSubscribtion(String channel,int subId){
        boolean isRemoved=false;
        if(!subsribtion.containsKey(channel)){
            List<Integer> sub=subsribtion.get(channel);
            if (sub.contains(subId)) {
                sub.remove(subId);
                isRemoved = true;
            }
        }
            return  isRemoved;
    }
    
}
