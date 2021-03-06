#include <occa/lang/expr/charNode.hpp>

namespace occa {
  namespace lang {
    charNode::charNode(token_t *token_,
                       const std::string &value_) :
      exprNode(token_),
      value(value_) {}

    charNode::charNode(const charNode &node) :
      exprNode(node.token),
      value(node.value) {}

    charNode::~charNode() {}

    udim_t charNode::type() const {
      return exprNodeType::char_;
    }

    exprNode* charNode::clone() const {
      return new charNode(token, value);
    }

    void charNode::setChildren(exprNodeRefVector &children) {}

    void charNode::print(printer &pout) const {
      pout << '\'' << escape(value, '\'') << '\'';
    }

    void charNode::debugPrint(const std::string &prefix) const {
      printer pout(std::cerr);
      std::cerr << prefix << '\n'
                << prefix << "|---[";
      pout << (*this);
      std::cerr << "] (char)\n";
    }
  }
}
