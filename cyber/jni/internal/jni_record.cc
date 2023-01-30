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

#ifdef WITH_JAVA

#include "cyber/python/internal/py_record.h"

#include <jni.h>
#include <limits>
#include <set>
#include <string>

using apollo::cyber::record::PyRecordReader;
using apollo::cyber::record::PyRecordWriter;

static std::string jstring_to_utf8(JNIEnv* env, jstring j_str) {
  jsize length = env->GetStringUTFLength(j_str);
  const char * const base = env->GetStringUTFChars(j_str, NULL);
  if (base == NULL) {
    return "";
  }
  std::string value(base, length);
  env->ReleaseStringUTFChars(j_str, base);
  return value;
}

extern "C" JNIEXPORT jlong JNICALL Java_com_apollo_RecordReader_createReader(
    JNIEnv *env, jclass cls, jstring filePath) {
  PyRecordReader *reader = new PyRecordReader(jstring_to_utf8(env, filePath));
  return (jlong)reader;
}

extern "C" JNIEXPORT void JNICALL Java_com_apollo_RecordReader_destroyReader(
    JNIEnv *env, jclass cls, jlong handle) {
  delete (PyRecordReader *)handle;
}

extern "C" JNIEXPORT jobject JNICALL Java_com_apollo_RecordReader_readMessage(
    JNIEnv *env, jclass cls, jlong handle, jlong beginTime, jlong endTime) {
  PyRecordReader *reader = (PyRecordReader *)handle;
  apollo::cyber::record::BagMessage msg = reader->ReadMessage(beginTime, endTime);
  if (msg.end) {
    return NULL;
  }
  jclass bagcls = env->FindClass("com/apollo/RecordReader$BagMessage");
  jmethodID constructor = env->GetMethodID(cls, "<init>", "()V");
  jobject ret = env->NewObject(bagcls, constructor);
  env->SetLongField(ret, env->GetFieldID(bagcls, "timestamp", "J"), msg.timestamp);
  env->SetObjectField(ret, env->GetFieldID(bagcls, "channelName", "Ljava/lang/String;"),
    env->NewStringUTF(msg.channel_name.c_str()));
  env->SetObjectField(ret, env->GetFieldID(bagcls, "dataType", "Ljava/lang/String;"),
    env->NewStringUTF(msg.data_type.c_str()));
  jbyteArray contents = env->NewByteArray(msg.data.length());
  env->SetByteArrayRegion(contents, 0, msg.data.length(), (const jbyte *)(msg.data.c_str()));
  env->SetObjectField(ret, env->GetFieldID(bagcls, "data", "[B"), contents);
  return ret;
}

extern "C" JNIEXPORT jint JNICALL Java_com_apollo_RecordReader_getMessageNumber(
    JNIEnv *env, jclass cls, jlong handle, jstring channelName) {
  PyRecordReader *reader = (PyRecordReader *)handle;
  return reader->GetMessageNumber(jstring_to_utf8(env, channelName));
}

extern "C" JNIEXPORT jstring JNICALL Java_com_apollo_RecordReader_getMessageType(
    JNIEnv *env, jclass cls, jlong handle, jstring channelName) {
  PyRecordReader *reader = (PyRecordReader *)handle;
  std::string message_type = reader->GetMessageType(jstring_to_utf8(env, channelName));
  return env->NewStringUTF(message_type.c_str());
}

extern "C" JNIEXPORT jbyteArray JNICALL Java_com_apollo_RecordReader_getProtoDesc(
    JNIEnv *env, jclass cls, jlong handle, jstring channelName) {
  PyRecordReader *reader = (PyRecordReader *)handle;
  std::string proto_desc = reader->GetProtoDesc(jstring_to_utf8(env, channelName));
  jbyteArray ret = env->NewByteArray(proto_desc.length());
  env->SetByteArrayRegion(ret, 0, proto_desc.length(), (const jbyte *)(proto_desc.c_str()));
  return ret;
}

extern "C" JNIEXPORT jbyteArray JNICALL Java_com_apollo_RecordReader_getHeaderString(
    JNIEnv *env, jclass cls, jlong handle) {
  PyRecordReader *reader = (PyRecordReader *)handle;
  std::string header_string = reader->GetHeaderString();
  jbyteArray ret = env->NewByteArray(header_string.length());
  env->SetByteArrayRegion(ret, 0, header_string.length(), (const jbyte *)(header_string.c_str()));
  return ret;
}

extern "C" JNIEXPORT void JNICALL Java_com_apollo_RecordReader_reset(
    JNIEnv *env, jclass cls, jlong handle) {
  PyRecordReader *reader = (PyRecordReader *)handle;
  reader->Reset();
}

extern "C" JNIEXPORT jobjectArray JNICALL Java_com_apollo_RecordReader_getChannelList(
    JNIEnv *env, jclass cls, jlong handle) {
  PyRecordReader *reader = (PyRecordReader *)handle;
  std::set<std::string> channel_list = reader->GetChannelList();
  jclass strcls = env->FindClass("java/lang/String");
  jobjectArray ret = env->NewObjectArray(channel_list.size(), strcls, NULL);
  int i = 0;
  for (std::set<std::string>::iterator it = channel_list.begin(); it != channel_list.end(); ++it) {
    env->SetObjectArrayElement(ret, i++, env->NewStringUTF(it->c_str()));
  }
  return ret;
}

#endif
