package bgu.spl.net.srv;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionImp<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler> clients; //<connction id, connection handler>
    private ConcurrentHashMap<String, List<Integer>> subsribtion; //<channel name, list<connection id>>
    private ConcurrentHashMap<Integer, Boolean> login; //<connection id, is logged in>
    private ConcurrentHashMap<Integer,String> idData;//<connection id, password>

    public ConnectionImp(){
        this.clients=new ConcurrentHashMap<>();
        this.login=new ConcurrentHashMap<>();
        this.subsribtion=new ConcurrentHashMap<>();
        this.idData=new ConcurrentHashMap<>();
    }
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
        subsribtion.forEach((key, list) -> {
            synchronized (list) { // Synchronize to avoid concurrent modification
                list.removeIf(id -> id == connectionId);
            }
        });
        login.computeIfPresent(connectionId, (key, value) -> false);
    }

    public void addConnect(int connectionId,ConnectionHandler ch){
        clients.put(connectionId,ch);
    }

    public void addSubscribtion(String channel,int subId){
        if (!subsribtion.containsKey(channel))
            subsribtion.put(channel, new ArrayList<>());
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
    public boolean isLoggedIn(int connectionId){
        return login.get(connectionId)!=null && login.get(connectionId);
    }

    public boolean isCorrectPassword(int id, String password){
        if(login.get(id)!=null){
            login.put(id,true); 
            idData.put(id,password);
            return true;
        }else
            return login.get(id).equals(password);
    }

    public boolean isSubscribe(int id, String channelName){
        if(subsribtion.get(channelName)!=null)
            for(Integer subId:subsribtion.get(channelName)){
                if(subId==id)
                    return true;
            }
        return false;
    }
    
}
