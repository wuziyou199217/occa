#include <occa/lang/expr/deleteNode.hpp>
#include <occa/lang/expr/parenthesesNode.hpp>

namespace occa {
  namespace lang {
    deleteNode::deleteNode(token_t *token_,
                           const exprNode &value_,
                           const bool isArray_) :
      exprNode(token_),
      value(value_.clone()),
      isArray(isArray_) {}

    deleteNode::deleteNode(const deleteNode &node) :
      exprNode(node.token),
      value(node.value->clone()),
      isArray(node.isArray) {}

    deleteNode::~deleteNode() {
      delete value;
    }

    udim_t deleteNode::type() const {
      return exprNodeType::delete_;
    }

    exprNode* deleteNode::clone() const {
      return new deleteNode(token, *value, isArray);
    }

    exprNode* deleteNode::endNode() {
      return value->endNode();
    }

    void deleteNode::setChildren(exprNodeRefVector &children) {
      children.push_back(&value);
    }

    exprNode* deleteNode::wrapInParentheses() {
      return new parenthesesNode(token, *this);
    }

    void deleteNode::print(printer &pout) const {
      pout << "delete ";
      if (isArray) {
        pout << "[] ";
      }
      pout << *value;
    }

    void deleteNode::debugPrint(const std::string &prefix) const {
      printer pout(std::cerr);
      std::cerr << prefix << '\n'
                << prefix << "|---[";
      pout << *value;
      std::cerr << "] (delete";
      if (isArray) {
        std::cerr << " []";
      }
      std::cerr << ")\n";
    }
  }
}
