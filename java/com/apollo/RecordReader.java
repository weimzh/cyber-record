/******************************************************************************
 * Copyright 2023, Wei Mingzhi <whistler_wmz@users.sf.net>.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

package com.apollo;

class RecordReader {
    static {
        System.loadLibrary("CyberRecordWrapper");
    }

    public static class BagMessage {
        public long timestamp;
        public String channelName;
        public byte[] data;
        public String dataType;
    }

    public static native long createReader(String filePath);

    public static native void destroyReader(long handle);

    public static native BagMessage readMessage(long handle, long beginTime, long endTime);

    public static BagMessage readMessage(long handle, long beginTime) {
        return readMessage(handle, beginTime, Long.MAX_VALUE);
    }

    public static BagMessage readMessage(long handle) {
        return readMessage(handle, 0);
    }

    public static native int getMessageNumber(long handle, String channelName);

    public static native String getMessageType(long handle, String channelName);

    public static native byte[] getProtoDesc(long handle, String channelName);

    public static native byte[] getHeaderString(long handle);

    public static native void reset(long handle);

    public static native String[] getChannelList(long handle);
}
