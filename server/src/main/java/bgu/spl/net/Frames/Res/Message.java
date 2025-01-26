package bgu.spl.net.Frames.Res;

import bgu.spl.net.Frames.Frame;

import java.util.concurrent.ConcurrentHashMap;

public class Message extends ResponseFrame {

    public Message(String msg, String destination, Integer subscription, int msgId){
        super("MESSAGE", Frame.getData("MESSAGE\n subscription:"+subscription+"\n message-id:"+msgId+"\n destination:"+destination+"\n\n"),msg);
    }



}
