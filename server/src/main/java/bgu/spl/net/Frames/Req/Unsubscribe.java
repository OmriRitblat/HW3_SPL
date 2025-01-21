package bgu.spl.net.Frames.Req;

import bgu.spl.net.srv.ConnectionImp;

public class Unsubscribe extends RequestFrame {
    private String channelName;
    private int id;


    public Unsubscribe(String msg) {
        super(-1);
        String[] lines = msg.split("\n");
        int recipt=-1;
        for(String line : lines){
            int colonIndex = msg.indexOf(":");
            String key = msg.substring(0, colonIndex).trim();
            // Extract the value (after the colon)
            String value = msg.substring(colonIndex + 1);
            if(key=="recipt")
                recipt = Integer.parseInt(value);
            else if(key=="destination")
                channelName = value;
            else if(key=="id")
                id = Integer.parseInt(value);
        }
        this.setRecipet(recipt);
    }

    @Override
    public void process(int id, ConnectionImp c) {
        //add the number of id to the map of user and there id for channel
        c.removeSubscribtion(channelName,id);
    }
}
