#ifndef MPICheckerAST_HPP_O1KSUWZO
#define MPICheckerAST_HPP_O1KSUWZO

#include "../ClangSACheckers.h"
#include "MPIFunctionClassifier.hpp"
#include "MPIBugReporter.hpp"
#include "Container.hpp"
#include "Utility.hpp"

namespace mpi {

/**
 * Class to implement the actual checks.
 */
class MPICheckerAST : public clang::RecursiveASTVisitor<MPICheckerAST> {
public:
    MPICheckerAST(clang::ento::BugReporter &bugReporter,
                  const clang::ento::CheckerBase &checkerBase,
                  clang::ento::AnalysisManager &analysisManager)
        : funcClassifier_{analysisManager},
          bugReporter_{bugReporter, checkerBase, analysisManager},
          analysisManager_{analysisManager} {}

    void checkPointToPointSchema();
    void checkReachbility();
    void checkForRedundantCalls() const;
    void checkForCollectiveCall(const MPICall &) const;
    void checkForInvalidArgs(const MPICall &) const;
    void checkBufferTypeMatch(const MPICall &mpiCall) const;
    typedef llvm::SmallVector<std::pair<size_t, size_t>, 2> IndexPairs;
    IndexPairs bufferDataTypeIndices(const MPICall &) const;

    MPIFunctionClassifier funcClassifier_;
    MPIBugReporter bugReporter_;

private:
    bool isSendRecvPair(const MPICall &, const MPICall &) const;
    void checkUnmatchedCalls() const;
    void checkSendRecvMatches(const MPIRankCase &, const MPIRankCase &);
    void checkReachbilityPair(const MPIRankCase &, const MPIRankCase &);
    void checkForRedundantCall(const MPICall &callToCheck,
                               const MPIRankCase &) const;
    bool qualifyRedundancyCheck(const MPICall &, const MPICall &) const;
    std::vector<size_t> integerIndices(const MPICall &) const;

    void selectTypeMatcher(const mpi::TypeVisitor &, const MPICall &,
                           const clang::StringRef,
                           const std::pair<size_t, size_t> &) const;
    bool matchBoolType(const mpi::TypeVisitor &, const llvm::StringRef) const;
    bool matchCharType(const mpi::TypeVisitor &, const llvm::StringRef) const;
    bool matchSignedType(const mpi::TypeVisitor &, const llvm::StringRef) const;
    bool matchUnsignedType(const mpi::TypeVisitor &,
                           const llvm::StringRef) const;
    bool matchFloatType(const mpi::TypeVisitor &, const llvm::StringRef) const;
    bool matchComplexType(const mpi::TypeVisitor &,
                          const llvm::StringRef) const;
    bool matchExactWidthType(const mpi::TypeVisitor &,
                             const llvm::StringRef) const;

    clang::ento::AnalysisManager &analysisManager_;
};

}  // end of namespace: mpi

#endif  // end of include guard: MPICheckerAST_HPP_O1KSUWZO
