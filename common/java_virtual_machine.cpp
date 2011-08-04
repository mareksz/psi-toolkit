#include "java_virtual_machine.hpp"

jint const JavaVirtualMachine::JNI_VERSION = JNI_VERSION_1_6;
string JavaVirtualMachine::javaClassPath = "../common/java:../common/java/morfologik-stemming-1.5.0.jar";
string JavaVirtualMachine::javaLibraryPath = "";

JavaVirtualMachine* JavaVirtualMachine::jvmInstance = NULL;

JavaVirtualMachine* JavaVirtualMachine::Instance () {
  if (!jvmInstance) {
  	jvmInstance = new JavaVirtualMachine();
  }   
	return jvmInstance;
}
     
JavaVirtualMachine::JavaVirtualMachine() {	// constructor
	javaVM = NULL;
	inited = false;
	jniENV = create(&javaVM);
}

JNIEnv* JavaVirtualMachine::create(JavaVM ** jvm) {
	JNIEnv *env;
    JavaVMInitArgs vm_args;
    JavaVMOption options[2];
  
	string cp = "-Djava.class.path=" + string(javaClassPath);
	options[0].optionString = (char *)cp.c_str();
	string lp = "-Djava.library.path=" + string(javaLibraryPath);
	options[1].optionString = (char *)lp.c_str();

    vm_args.version = JNI_VERSION; 
    vm_args.nOptions = 2;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = 0;
      
    int result = JNI_CreateJavaVM(jvm, (void**)&env, &vm_args);
    if (result < 0) {
		// it should be an exception
        printf("Unable to Launch JVM\n");
	}
	else {
		inited = true;
	}
    return env;
}

JavaVM* JavaVirtualMachine::getJVM() {
	return javaVM;
}
JNIEnv* JavaVirtualMachine::getENV() {
	return jniENV;
}

void JavaVirtualMachine::setJavaClassPath(string classPath) {
	javaClassPath = classPath;
}
void JavaVirtualMachine::setJavaLibraryPath(string libraryPath) {
	javaLibraryPath = libraryPath;
}

