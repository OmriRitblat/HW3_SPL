package bgu.spl.net.srv;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

public class ConnectionImp<T> implements Connections<T>{
    private ConcurrentHashMap<Integer, ConnectionHandler> clients; //<connction id, connection handler>
    private ConcurrentHashMap<String, List<Integer>> subsribtion; //<channel name, list<connection id>>
    private ConcurrentHashMap<String, UserInfo> users; //<user name, user info>
    private ConcurrentHashMap<Integer, String> clientToUser; //<connection id, user name>
    private AtomicInteger messageCount;

    public ConnectionImp(){
        this.clients=new ConcurrentHashMap<>();
        this.subsribtion=new ConcurrentHashMap<>();
        this.users=new ConcurrentHashMap<>();
        this.messageCount=new AtomicInteger(0);
        this.clientToUser=new ConcurrentHashMap<>();
    }
    public boolean send(int connectionId, T msg){
        ConnectionHandler c=clients.get(connectionId);
        if(c!=null) {
            System.out.println((String)msg);
            c.send(msg);
            return true;
        }
        return false;
    }

    public void send(String channel, T msg){
        List<Integer> sub=subsribtion.get(channel);
        synchronized(sub){
        if(sub!=null)
            for(Integer i:sub) {
                send(i, msg);
            }
        notifyAll();
        }
    }

    public void disconnect(int connectionId){
        subsribtion.forEach((key, list) -> {
            synchronized (list) { // Synchronize to avoid concurrent modification
                list.removeIf(id -> id == connectionId);
                notifyAll();
            }
        });
        users.get(clientToUser.get(connectionId)).setLoggedIn(false);
        if(clients.get(connectionId)!=null){
            try {
                clients.get(connectionId).close();
                clients.remove(connectionId);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
        clients.remove(connectionId);
    }

    public int getMessageCount() {
        return messageCount.addAndGet(1);
    }

    public List<Integer> getSubsribtion(String channel){
        return subsribtion.get(channel);
    }

    public Integer getChannelId(String channel,int connectionId){
        return users.get(clientToUser.get(connectionId)).getChannelId(channel);
    }

    public void addConnect(int connectionId,ConnectionHandler ch){
        clients.put(connectionId,ch);
    }

    public void addSubscribtion(String channel,int connectionId, int channelId){
        if (!subsribtion.containsKey(channel)){
            subsribtion.put(channel, new ArrayList<>());
        }
        subsribtion.get(channel).add(connectionId);
        if(users.get(clientToUser.get(connectionId)).getChannelName(channelId)==null)
            users.get(clientToUser.get(connectionId)).addSubscribtion(channelId,channel);
    }

    public boolean removeSubscribtion(int channelId,int subId){
        boolean isRemoved=false;
        if(clientToUser.get(subId)!=null && users.get(clientToUser.get(subId))!=null) {
            if (users.get(clientToUser.get(subId)).getChannelName(channelId) != null) {
                String channelName = users.get(clientToUser.get(subId)).getChannelName(channelId);
                if (subsribtion.containsKey(channelName)) {
                    List<Integer> sub = subsribtion.get(channelName);
                    if (sub != null && sub.contains(subId)) {
                        subsribtion.get(channelName).remove(subId);
                        if (users.get(clientToUser.get(subId)).removeSubscribtion(channelId))
                            isRemoved = true;
                    }
                }
            }
        }
            return  isRemoved;
    }
    public boolean isLoggedIn(int connectionId){
        boolean result=false;
        if(clientToUser.get(connectionId)!=null)
            if(users.get(clientToUser.get(connectionId))!=null)
                result=users.get(clientToUser.get(connectionId)).isLoggedIn();
        return result;
    }

    public boolean isCorrectPassword(int id, String password){
        if(clientToUser.get(id)!=null && users.get(clientToUser.get(id))!=null) {
            if (users.get(clientToUser.get(id)).isLoggedIn()) {
                users.get(clientToUser.get(id)).setPasscode(password);
                return true;
            } else
                return users.get(clientToUser.get(id)).getPasscode().equals(password);
        }
        return true;
    }

    public void login(int connectionId, String userName){
        users.put(userName,new UserInfo());
        clientToUser.put(connectionId,userName);
    }

    public boolean isSubscribe(int id, int channelId){
        if(clientToUser.get(id)!=null && users.get(clientToUser.get(id))!=null) {
            return users.get(clientToUser.get(id)).isSubscribe(channelId);
        }
        return false;
    }

    public boolean isSubscribe(int id, String channelName){
        if(clientToUser.get(id)!=null && users.get(clientToUser.get(id))!=null) {
            return users.get(clientToUser.get(id)).isSubscribe(channelName);
        }
        return false;
    }


    
}
