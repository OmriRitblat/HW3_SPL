package bgu.spl.net.Frames.Req;

import bgu.spl.net.Frames.Res.Connected;
import bgu.spl.net.Frames.Res.Error;
import bgu.spl.net.Frames.Res.ResponseFrame;
import bgu.spl.net.srv.ConnectionImp;

public class Connect extends RequestFrame {
    private String verstion;
    private String host;
    private String user;
    private String password;
    private String msg;

    public Connect(String msg){
        super(-1);
        this.msg = msg;
        String[] lines = msg.split("\n");
        int recipt=-1;
        for(String line : lines){
            int colonIndex = msg.indexOf(":");
            String key = msg.substring(0, colonIndex).trim();
            // Extract the value (after the colon)
            String value = msg.substring(colonIndex + 1);
            if(key=="recipt")
                recipt = Integer.parseInt(value);
            else if(key=="host")
                host = value;
            else if(key=="user")
                user = value;
            else if(key=="password")
                password = value;
        }
        this.setRecipet(recipt);
    }

    @Override
    public void process(int id, ConnectionImp c) {
        ResponseFrame f;
        if(c.isLoggedIn(id))
            f=new Error("User already logged in",msg,"User already logged in",this.getReciept());
        else
            f=new Connected(verstion);
        c.send(id,f);
    }
}
