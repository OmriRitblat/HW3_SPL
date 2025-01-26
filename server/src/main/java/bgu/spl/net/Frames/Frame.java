package bgu.spl.net.Frames;

import java.io.Serializable;
import java.util.Map;
import java.util.Scanner;
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
    @Override
    public String toString() {
        StringBuilder output = new StringBuilder();

        // Append type
        output.append(type).append("\n");

        // Append headers
        if (headers != null) {
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                output.append(entry.getKey())
                    .append(":")
                    .append(entry.getValue())
                    .append("\n");
            }
        }

        // Append body
        output.append("\n")
            .append(body == null ? "null" : body);

        return output.toString();
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
    public boolean headersCheck(int numOfHeaders){//without receipt
        int counter=0;
        for (Map.Entry<String, String> entry : headers.entrySet()) {
            if(!entry.getKey().equals("receipt"))
                counter++;
        }
        return counter==numOfHeaders;
    }
}
