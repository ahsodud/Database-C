#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "Database_in_C.c"

// Test function for createKeyValue
void test_createKeyValue(void) {
    KeyValue *kv = createKeyValue("name", "Alice");
    CU_ASSERT_STRING_EQUAL(kv->key, "name");
    CU_ASSERT_STRING_EQUAL(kv->value, "Alice");
    CU_ASSERT_PTR_NULL(kv->next);
    freeKeyValue(kv);
}

// Test function for createBSTNode
void test_createBSTNode(void) {
    KeyValue *kv = createKeyValue("name", "Alice");
    BSTNode *node = createBSTNode(1, kv);
    CU_ASSERT_EQUAL(node->id, 1);
    CU_ASSERT_PTR_EQUAL(node->data, kv);
    CU_ASSERT_PTR_NULL(node->left);
    CU_ASSERT_PTR_NULL(node->right);
    freeBST(node);
}

// Test function for freeKeyValue
void test_freeKeyValue(void) {
    KeyValue *kv = createKeyValue("name", "Alice");
    freeKeyValue(kv);
    // No direct assertion possible, ensure no memory leaks
}

// Test function for freeBST
void test_freeBST(void) {
    KeyValue *kv1 = createKeyValue("name", "Alice");
    KeyValue *kv2 = createKeyValue("age", "30");
    kv1->next = kv2;
    BSTNode *node = createBSTNode(1, kv1);
    freeBST(node);
    // No direct assertion possible, ensure no memory leaks
}

// Test function for insertIntoBST
void test_insertIntoBST(void) {
    KeyValue *kv1 = createKeyValue("name", "Alice");
    BSTNode *root = createBSTNode(1, kv1);
    KeyValue *kv2 = createKeyValue("name", "Bob");
    BSTNode *node = createBSTNode(2, kv2);
    root = insertIntoBST(root, node, "name");
    CU_ASSERT_PTR_NOT_NULL(root->right);
    CU_ASSERT_STRING_EQUAL(root->right->data->value, "Bob");
    freeBST(root);
}

// Test function for searchBST
void test_searchBST(void) {
    KeyValue *kv1 = createKeyValue("name", "Alice");
    BSTNode *root = createBSTNode(1, kv1);
    KeyValue *kv2 = createKeyValue("name", "Bob");
    BSTNode *node = createBSTNode(2, kv2);
    root = insertIntoBST(root, node, "name");

    int found = 0;
    int result = searchBST(root, "name", "Bob", 1, &found);
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_EQUAL(found, 1);
    freeBST(root);
}

int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("a3_test_suite", 0, 0);
    CU_add_test(suite, "test_createKeyValue", test_createKeyValue);
    CU_add_test(suite, "test_createBSTNode", test_createBSTNode);
    CU_add_test(suite, "test_freeKeyValue", test_freeKeyValue);
    CU_add_test(suite, "test_freeBST", test_freeBST);
    CU_add_test(suite, "test_insertIntoBST", test_insertIntoBST);
    CU_add_test(suite, "test_searchBST", test_searchBST);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
