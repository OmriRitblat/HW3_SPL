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
    private ConcurrentHashMap<Integer, ConcurrentHashMap<Integer,String>> channelsId; // <connection id, <channel id, channel name>>
    private ConcurrentHashMap<Integer, Boolean> login; //<connection id, is logged in>
    private ConcurrentHashMap<Integer,String> idData;//<connection id, password>
    private AtomicInteger messageCount;

    public ConnectionImp(){
        this.clients=new ConcurrentHashMap<>();
        this.login=new ConcurrentHashMap<>();
        this.subsribtion=new ConcurrentHashMap<>();
        this.idData=new ConcurrentHashMap<>();
        this.channelsId=new ConcurrentHashMap<>();
        this.messageCount=new AtomicInteger(0);
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
        login.computeIfPresent(connectionId, (key, value) -> false);
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
        if(channelsId.containsKey(connectionId))
            return findKeyByValue(channelsId.get(connectionId), channel);
        return null;
    }

    public void addConnect(int connectionId,ConnectionHandler ch){
        clients.put(connectionId,ch);
    }

    public void addSubscribtion(String channel,int connectionId, int channelId){
        if (!subsribtion.containsKey(channel)){
            subsribtion.put(channel, new ArrayList<>());
            if (!channelsId.containsKey(connectionId)){
                channelsId.put(connectionId,new ConcurrentHashMap<>());
            }
        }
        subsribtion.get(channel).add(connectionId);
        channelsId.get(connectionId).put(channelId,channel);
    }

    public boolean removeSubscribtion(int channelId,int subId){
        boolean isRemoved=false;
        if(channelsId.containsKey(subId)){
            if(channelsId.get(subId).containsKey(channelId)) {
                String channelName = channelsId.get(subId).get(channelId);
                channelsId.get(subId).remove(channelId);
                if(subsribtion.containsKey(channelName)){
                    List<Integer> sub=subsribtion.get(channelName);
                    if (sub.contains(subId)) {
                        sub.remove(subId);
                        isRemoved = true;
                    }
                }
            }
        }

            return  isRemoved;
    }
    public boolean isLoggedIn(int connectionId){
        return login.get(connectionId)!=null && login.get(connectionId);
    }

    public boolean isCorrectPassword(int id, String password){
        if(login.get(id)==null){
            idData.put(id,password);
            return true;
        }else
            return idData.get(id).equals(password);
    }

    public void login(int connectionId){
        login.put(connectionId,true);
    }

    public boolean isSubscribe(int id, int channelId){
        if(channelsId.containsKey(id)) {
            if (channelsId.get(id).containsKey(channelId)) {
                String channelName = channelsId.get(id).get(channelId);
                if (subsribtion.get(channelName) != null)
                    for (Integer subId : subsribtion.get(channelName)) {
                        if (subId == id)
                            return true;
                    }
            }
        }
        return false;
    }

    public boolean isSubscribe(int id, String channelName){
        if(channelsId.containsKey(id)) {
            Integer channelId= findKeyByValue(channelsId.get(id), channelName);
            if (channelId!=null) {
                return true;
            }
        }
        return false;
    }

    private Integer findKeyByValue(ConcurrentHashMap<Integer, String> map, String valueReq) {
        AtomicInteger keyForValueReq = new AtomicInteger(-1);
        map.forEach((key, value) -> {
            if (value.equals(valueReq)) {
                keyForValueReq.updateAndGet(v -> key);
            }
        });
        if(keyForValueReq.get()!=-1){
            return keyForValueReq.get();
        }
        return null;
    }
    
}
