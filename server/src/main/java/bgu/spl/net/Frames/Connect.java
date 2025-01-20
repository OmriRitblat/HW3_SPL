package bgu.spl.net.Frames;

import bgu.spl.net.srv.ConnectionImp;

public class Connect extends Frame {
    private String verstion;
    private String host;
    private String user;
    private String password;

    public Connect(String msg){
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
    }
}
