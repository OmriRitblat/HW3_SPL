package bgu.spl.net.srv;

import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class UserInfo {
    private boolean isLoggedIn;
    private String passcode;
    private ConcurrentHashMap<Integer, String> channelsId; //<channel id, channel name>
    
    public UserInfo(){
        this.isLoggedIn=true;
        this.passcode="";
        this.channelsId=new ConcurrentHashMap<>();
    }

    public void setPasscode(String passcode){
        this.passcode=passcode;
    }
    public String getPasscode(){
        return this.passcode;
    }
    public boolean isLoggedIn(){
        return this.isLoggedIn;
    }
    public void setLoggedIn(boolean loggedIn){
        this.isLoggedIn=loggedIn;
    }

    public Integer getChannelId(String name){
        AtomicInteger keyForValueReq = new AtomicInteger(-1);
        channelsId.forEach((key, value) -> {
            if (value.equals(name)) {
                keyForValueReq.updateAndGet(v -> key);
            }
        });
        if(keyForValueReq.get()!=-1){
            return keyForValueReq.get();
        }
        return null;
    }

    public void addSubscribtion(int channelId, String name){
        this.channelsId.put(channelId, name);
    }

    public String getChannelName(int channelId){
        return this.channelsId.get(channelId);
    }

    public boolean removeSubscribtion(int channelId){
        if(this.channelsId.containsKey(channelId)){
            this.channelsId.remove(channelId);
            return true;
        }
        return false;
    }

    public boolean isSubscribe(int channelId){
        return this.channelsId.containsKey(channelId);
    }

    public boolean isSubscribe(String channelName){
        Integer channelId = this.getChannelId(channelName);
        if(channelId!=null)
            return this.isSubscribe(channelId);
        return false;
    }



    
}
