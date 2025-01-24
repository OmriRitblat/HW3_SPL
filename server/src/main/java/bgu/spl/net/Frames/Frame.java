package bgu.spl.net.Frames;

import java.io.Serializable;
import java.util.concurrent.ConcurrentHashMap;

import bgu.spl.net.srv.Connections;

public abstract class Frame implements Serializable {
    protected String type;
    protected ConcurrentHashMap<String, String> headers;
    protected String body;

    public Frame(String type, ConcurrentHashMap<String, String> headers, String body) {
        this.type = type;
        this.headers = headers;
        this.body = body;
    }

    public String getType() {
        return type;
    }

    public ConcurrentHashMap<String, String> getHeaders() {
        return headers;
    }

    public String getBody() {
        return body;
    }
    public String getHeaderByKey(String key){
        if(!headers.containsKey(key))
            return "";
        return headers.get(key);
    }
    public String toString(){
        String output = type + "\n";
        for (String key : headers.keySet()){
            output += key + ":" + headers.get(key) + "\n";
        }
        output += "\n" + body;
        return output;
    }  
}
