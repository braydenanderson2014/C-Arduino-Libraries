/*
  Example: AVLTree basic usage

  Demonstrates insertion, lookup, traversal, min/max retrieval,
  deletion, and clear behavior.
*/

#include <AVLTree.h>

AVLTree<int> tree;

void setup() {
  Serial.begin(9600);

  tree.insert(10);
  tree.insert(20);
  tree.insert(30);
  tree.insert(40);
  tree.insert(50);
  tree.insert(25);

  Serial.println("In-order traversal:");
  tree.inOrder();

  Serial.print("Size: ");
  Serial.println(static_cast<unsigned long>(tree.size()));

  Serial.print("Contains 25: ");
  Serial.println(tree.contains(25));

  Serial.print("Minimum: ");
  Serial.println(tree.findMin());
  Serial.print("Maximum: ");
  Serial.println(tree.findMax());

  tree.remove(30);

  Serial.println("In-order traversal after deleting 30:");
  tree.inOrder();

  Serial.print("Tree height: ");
  Serial.println(tree.height());
  Serial.print("Tree root balance: ");
  Serial.println(tree.getBalance());

  tree.clear();
  Serial.print("Empty after clear: ");
  Serial.println(tree.isEmpty());
}

void loop() {}
