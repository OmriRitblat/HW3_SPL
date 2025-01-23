package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;

import java.io.*;
import java.nio.ByteBuffer;

public class MessageEncoderDecoderImpl implements MessageEncoderDecoder<String> {

    private final ByteBuffer lengthBuffer = ByteBuffer.allocate(4);
    private byte[] objectBytes = null;
    private int objectBytesIndex = 0;


    @Override
    public String decodeNextByte(byte nextByte) {
        if (objectBytes == null) { // Indicates that we are still reading the length
            lengthBuffer.put(nextByte);
            if (!lengthBuffer.hasRemaining()) { // We have read 4 bytes, so we can determine the object length
                lengthBuffer.flip();
                int objectLength = lengthBuffer.getInt();
                objectBytes = new byte[objectLength];
                objectBytesIndex = 0;
                lengthBuffer.clear();
            }
        } else {
            objectBytes[objectBytesIndex++] = nextByte;
            if (objectBytesIndex == objectBytes.length) { // Entire object has been read
                Serializable result = deserializeObject(objectBytes);
                objectBytes = null; // Reset for the next object
                return result.toString(); // Ensure you convert to String before returning
            }
        }

        return null; // Return null until a complete object is ready
    }


    @Override
    public byte[] encode(String message) {
        return serializeObject(message);
    }

    private Serializable deserializeObject(byte[] objectBytes) {
        try {
            ObjectInput in = new ObjectInputStream(new ByteArrayInputStream(this.objectBytes));
            return (Serializable) in.readObject();
        } catch (Exception ex) {
            throw new IllegalArgumentException("cannot desrialize object", ex);
        }

    }

    private byte[] serializeObject(Serializable message) {
        try {
            ByteArrayOutputStream bytes = new ByteArrayOutputStream();

            //placeholder for the object size
            for (int i = 0; i < 4; i++) {
                bytes.write(0);
            }

            ObjectOutput out = new ObjectOutputStream(bytes);
            out.writeObject(message);
            out.flush();
            byte[] result = bytes.toByteArray();

            //now write the object size
            ByteBuffer.wrap(result).putInt(result.length - 4);
            return result;

        } catch (Exception ex) {
            throw new IllegalArgumentException("cannot serialize object", ex);
        }
    }

}
