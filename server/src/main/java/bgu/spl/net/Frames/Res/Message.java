package bgu.spl.net.Frames.Res;

import bgu.spl.net.Frames.Req.RequestFrame;
import bgu.spl.net.srv.ConnectionImp;

public class Message extends ResponseFrame {
    private String channelName;
    private String user;
    private String city;
    private String eventName;
    private String dateTime;
    private String active;
    private String forces;
    private String description;
    private int msgId;

    public Message(String msg){
        super("MESSAGE");
        String[] lines = msg.split("\n");
        this.msgId=-1;
        boolean isDesciption=false;
        StringBuilder descrip = new StringBuilder();
        for(int i=0;i<lines.length;i++){
            int colonIndex = lines[i].indexOf(":");
            String key = lines[i].substring(0, colonIndex);
            // Extract the value (after the colon)
            String value = lines[i].substring(colonIndex + 1);
            if(key=="description" || isDesciption){
                isDesciption=true;
                descrip.append(value);
            }else{
                if(key=="user")
                    user = value;
                else if (key=="destination") {
                    channelName=value;
                } else if(key=="city")
                    city = value;
                else if(key=="event name")
                    eventName = value;
                else if(key=="date time")
                    dateTime = value;
                else if(key=="general information"){
                     colonIndex = lines[i+1].indexOf(":");
                     active = lines[i+1].substring(colonIndex + 1);
                     colonIndex = lines[i+2].indexOf(":");
                     forces= lines[i+2].substring(colonIndex + 1);
                     i++;
                }
            }
        }
        this.description = descrip.toString();
    }


}
