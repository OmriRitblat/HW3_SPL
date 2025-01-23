package bgu.spl.net.impl.stomp;

import java.nio.charset.StandardCharsets;
import java.util.Arrays;

import bgu.spl.net.api.MessageEncoderDecoder;

public class StompEncoderDecoder implements MessageEncoderDecoder<String> {
    private byte[] bytes = new byte[1 << 10]; //1024
    private int len = 0;


    public String decodeNextByte(byte nextByte){
        if (nextByte == 0) {
            return popString();
        }
        pushByte(nextByte);
        return null;
    }

    public byte[] encode(String message){
        return (message + '\u0000').getBytes();
    }

    private void pushByte(byte nextByte){
        if (len >= bytes.length) {
            bytes = Arrays.copyOf(this.bytes, this.len * 2);
        }
        bytes[len++] = nextByte;
    }

    private String popString(){
         String result = new String(bytes, 0, len, StandardCharsets.UTF_8);
        len = 0;
        return result;
    }
}
