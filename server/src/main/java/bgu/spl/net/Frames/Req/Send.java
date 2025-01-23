package bgu.spl.net.Frames.Req;

import bgu.spl.net.Frames.Res.Error;
import bgu.spl.net.Frames.Res.Message;
import bgu.spl.net.Frames.Res.ResponseFrame;
import bgu.spl.net.srv.ConnectionImp;

public class Send extends RequestFrame {
    private String channelName;
    private String body;

    public Send(String msg) {
        super(-1,msg);
        String[] lines = msg.split("\n");
        int receipt = -1;
        boolean isDesciption = false;
        StringBuilder descrip = new StringBuilder();
        for (int i = 0; i < lines.length; i++) {
            if (lines[i] != "\n") {
                int colonIndex = lines[i].indexOf(":");
                String key = lines[i].substring(0, colonIndex);
                // Extract the value (after the colon)
                String value = lines[i].substring(colonIndex + 1);
                if (isDesciption) {
                    descrip.append(value);
                } else if (key == "destination")
                    channelName = value;
            } else
                isDesciption = true;
        }
        this.body = descrip.toString();
        setRecipet(receipt);
    }

    @Override
    public void process(int id, ConnectionImp c) {
        ResponseFrame f;
        if(!c.isSubscribe(id,channelName)){
            f=new Error("user not subscribe to the channel or the cannel does not exist", getMessage(), "user not subscribe to the channel or the cannel does not exist", getReciept());
            c.send(id,f);
        }
        else{
            f=new Message(body);
            c.send(channelName, f);
        }
    }
}
