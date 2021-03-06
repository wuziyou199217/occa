#include <occa/tools/string.hpp>
#include <occa/lang/mode/cuda.hpp>
#include <occa/lang/mode/okl.hpp>
#include <occa/lang/mode/oklForStatement.hpp>
#include <occa/lang/builtins/attributes.hpp>
#include <occa/lang/builtins/types.hpp>

namespace occa {
  namespace lang {
    namespace okl {
      cudaParser::cudaParser(const occa::properties &settings_) :
        withLauncher(settings_),
        constant("__constant__", qualifierType::custom),
        global("__global__", qualifierType::custom),
        device("__device__", qualifierType::custom),
        shared("__shared__", qualifierType::custom) {

        okl::addAttributes(*this);
      }

      void cudaParser::onClear() {
        hostClear();
      }

      void cudaParser::beforePreprocessing() {
        preprocessor.addCompilerDefine("OCCA_USING_GPU", "1");
      }

      void cudaParser::beforeKernelSplit() {
        updateConstToConstant();

        if (!success) return;
        setFunctionQualifiers();

        if (!success) return;
        setSharedQualifiers();
      }

      void cudaParser::afterKernelSplit() {
        addBarriers();

        if (!success) return;
        setupKernels();
      }

      std::string cudaParser::getOuterIterator(const int loopIndex) {
        std::string name = "blockIdx.";
        name += 'x' + (char) loopIndex;
        return name;
      }

      std::string cudaParser::getInnerIterator(const int loopIndex) {
        std::string name = "threadIdx.";
        name += 'x' + (char) loopIndex;
        return name;
      }

      void cudaParser::updateConstToConstant() {
        const int childCount = (int) root.children.size();
        for (int i = 0; i < childCount; ++i) {
          statement_t &child = *(root.children[i]);
          if (child.type() != statementType::declaration) {
            continue;
          }
          declarationStatement &declSmnt = ((declarationStatement&) child);
          const int declCount = declSmnt.declarations.size();
          for (int di = 0; di < declCount; ++di) {
            variable_t &var = *(declSmnt.declarations[di].variable);
            if (var.has(const_)) {
              var -= const_;
              var += constant;
            }
          }
        }
      }

      void cudaParser::setFunctionQualifiers() {
        statementPtrVector funcSmnts;
        findStatementsByType(statementType::functionDecl,
                             root,
                             funcSmnts);

        const int funcCount = (int) funcSmnts.size();
        for (int i = 0; i < funcCount; ++i) {
          functionDeclStatement &funcSmnt = (
            *((functionDeclStatement*) funcSmnts[i])
          );
          if (funcSmnt.hasAttribute("kernel")) {
            continue;
          }
          vartype_t &vartype = funcSmnt.function.returnType;
          vartype.qualifiers.addFirst(vartype.origin(),
                                      device);
        }
      }

      void cudaParser::setSharedQualifiers() {
        statementExprMap exprMap;
        findStatements(statementType::declaration,
                       exprNodeType::variable,
                       root,
                       sharedVariableMatcher,
                       exprMap);

        statementExprMap::iterator it = exprMap.begin();
        while (it != exprMap.end()) {
          declarationStatement &declSmnt = *((declarationStatement*) it->first);
          const int declCount = declSmnt.declarations.size();
          for (int i = 0; i < declCount; ++i) {
            variable_t &var = *(declSmnt.declarations[i].variable);
            if (!var.hasAttribute("shared")) {
              continue;
            }
            var += shared;
          }
          ++it;
        }
      }

      void cudaParser::addBarriers() {
        statementPtrVector statements;
        findStatementsByAttr(statementType::empty,
                             "barrier",
                             root,
                             statements);

        const int count = (int) statements.size();
        for (int i = 0; i < count; ++i) {
          // TODO 1.1: Implement proper barriers
          emptyStatement &smnt = *((emptyStatement*) statements[i]);

          statement_t &barrierSmnt = (
            *(new expressionStatement(
                smnt.up,
                *(new identifierNode(smnt.source,
                                     " __syncthreads()"))
              ))
          );

          smnt.up->addBefore(smnt,
                             barrierSmnt);

          smnt.up->remove(smnt);
          delete &smnt;
        }
      }

      void cudaParser::setupKernels() {
        statementPtrVector kernelSmnts;
        findStatementsByAttr(statementType::functionDecl,
                             "kernel",
                             root,
                             kernelSmnts);

        const int kernelCount = (int) kernelSmnts.size();
        for (int i = 0; i < kernelCount; ++i) {
          functionDeclStatement &kernelSmnt = (
            *((functionDeclStatement*) kernelSmnts[i])
          );
          setKernelQualifiers(kernelSmnt);
          if (!success) return;
        }
      }

      void cudaParser::setKernelQualifiers(functionDeclStatement &kernelSmnt) {
        vartype_t &vartype = kernelSmnt.function.returnType;
        vartype.qualifiers.addFirst(vartype.origin(),
                                    global);
        vartype.qualifiers.addFirst(vartype.origin(),
                                    externC);
      }

      bool cudaParser::sharedVariableMatcher(exprNode &expr) {
        return expr.hasAttribute("shared");
      }
    }
  }
}
