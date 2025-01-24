package bgu.spl.net.Frames.Req;

import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.concurrent.ConcurrentHashMap;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.srv.ConnectionImp;
import bgu.spl.net.srv.Connections;

public abstract class RequestFrame extends Frame{
    private int reciept;
    private String msg;
    private boolean isMissingData;
    public RequestFrame(int reciept, String msg,String type) {
        super(type,RequestFrame.getData(msg),RequestFrame.getBody(msg));
        this.reciept = reciept;
        this.msg=msg;
        this.isMissingData=false;
    }

    public abstract void process(int id, ConnectionImp c);

    public void setRecipet(int reciept) {
        this.reciept = reciept;
    }
    public static ConcurrentHashMap<String, String> getData(String msg) {
        int delimiterPos = msg.indexOf("\n\n");
        String values = (delimiterPos != -1) ? msg.substring(0, delimiterPos) : "";
        // Parse the values
        Scanner valuesStream = new Scanner(values);
        if (valuesStream.hasNextLine()) {//get ride of the type line
            String typeLine = valuesStream.nextLine();
        }
        ConcurrentHashMap<String, String> data=new ConcurrentHashMap<>();
        // Parse key-value pairs from the remaining lines
        while (valuesStream.hasNextLine()) {
            String line = valuesStream.nextLine();
            if (!line.isEmpty()) {
                int pos = line.indexOf(':');
                if (pos != -1) {
                    String key = line.substring(0, pos).trim();
                    String value = line.substring(pos + 1).trim();
                        data.put(key, value);
                    }
            } else {
                break;
            }
        }
        valuesStream.close();
        return data;
    }
    public static String getBody(String msg){
        int delimiterPos = msg.indexOf("\n\n");
        String values = (delimiterPos != -1) ? msg.substring(0, delimiterPos) : "";
        return (delimiterPos != -1 && delimiterPos + 2 < msg.length())
                ? msg.substring(delimiterPos + 2)
                : "";
    }
    public int getReciept() {
        return reciept;
    }
    public String getMessage(){
        return msg;
    }
    public boolean isMissingData() {
        return isMissingData;
    }
    public void setMissingData(boolean isMissingData) {
        this.isMissingData = isMissingData;
    }

}
