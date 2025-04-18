#ifndef DO_API_NO_RETURN
#define DO_API_NO_RETURN(r, n, p) DO_API(r,n,p)
#endif

DO_API(FieldInfo*, il2cpp_class_get_field_from_name, (Il2CppClass * klass, const char *name));
DO_API(void, il2cpp_field_static_get_value, (FieldInfo * field, void *value));
DO_API(const MethodInfo*, il2cpp_class_get_methods, (Il2CppClass * klass, void* *iter));
DO_API(Il2CppClass*, il2cpp_class_from_type, (const Il2CppType * type));
DO_API(Il2CppObject*, il2cpp_object_new, (const Il2CppClass * klass));
DO_API(Il2CppThread*, il2cpp_thread_current, ());
DO_API(Il2CppThread*, il2cpp_thread_attach, (Il2CppDomain * domain));
DO_API(void, il2cpp_thread_detach, (Il2CppThread * thread));
DO_API(Il2CppDomain*, il2cpp_domain_get, ());
DO_API(const Il2CppAssembly*, il2cpp_domain_assembly_open, (const char* name));
DO_API(const Il2CppImage*, il2cpp_assembly_get_image, (const Il2CppAssembly * assembly));
DO_API(Il2CppString*, il2cpp_string_new, (const char* str));
DO_API(Il2CppClass*, il2cpp_class_from_system_type, (Il2CppReflectionType * type));
DO_API(FieldInfo*, il2cpp_class_get_fields, (Il2CppClass * klass, void* *iter));
DO_API(const PropertyInfo*, il2cpp_class_get_properties, (Il2CppClass * klass, void* *iter));
DO_API(Il2CppClass*, il2cpp_class_get_interfaces, (Il2CppClass * klass, void* *iter));
DO_API(Il2CppClass*, il2cpp_class_from_name, (const Il2CppImage * image, const char* namespaze, const char *name));
