#ifndef BZLA__LS_NODE_H
#define BZLA__LS_NODE_H

#include <vector>

#include "ls/ls.h"

namespace bzla {

class BitVector;
class RNG;

namespace ls {

template <class VALUE>
class Node
{
 public:
  /**
   * Path selection mode.
   * True if path is to be selected based on essential inputs, false if it is
   * to be selected randomly.
   */
  static inline bool s_path_sel_essential = true;
  /**
   * Probability for picking an essential input if there is one, and else
   * a random input (see use_path_sel_essential).
   */
  static inline uint32_t s_prob_pick_ess_input = 990;

  /** Destructor. */
  virtual ~Node();

  /**
   * Get the kind of the node.
   * @return The kind of this node.
   */
  virtual NodeKind kind() const { return NodeKind::CONST; }

  /**
   * Determine if this node is an inequality node.
   * @return True if this is an inequality node.
   */
  virtual bool is_inequality() const = 0;
  /**
   * Determine if this node is a not node.
   * @return True if this is a not node.
   */
  virtual bool is_not() const { return kind() == NodeKind::NOT; }

  /** Update assignment based on the assignment of its children. */
  virtual void evaluate(){};

  /**
   * Determine if this node is a value.
   * @note For bit-vector nodes, this checks if the underlying domain is fixed.
   * @return True if this node is a value.
   */
  bool is_value() const { return d_is_value; }
  /**
   * Determine if all children are values.
   * @return True if all children are values.
   */
  bool all_value() const { return d_all_value; }

  /**
   * Determine if this node is a false value.
   * @return True if this node is a value and represents false.
   */
  virtual bool is_value_false() const = 0;

  /**
   * Check if operand at index `pos_x` is essential with respect to constant
   * bits and target value `t`.
   *
   * @param t The target value.
   * @param pos_x The index of `x`.
   * @return True if operand at index `pos_x` is essential.
   */
  virtual bool is_essential(const VALUE& t, uint64_t pos_x);

  /**
   * Check invertibility condition for `x` at index `pos_x with` respect to
   * constant bits and target value `t`.
   *
   * Caches an inverse (if already determined while checking invertibility)
   * if `is_essential_check` is false.
   *
   * @param t The target value.
   * @param pos_x The index of `x`.
   * @param is_essential_check True if called to determine is_essential(). For
   *                           is_essential() checks, we don't consider bounds
   *                           derived from top-level inequalities since this
   *                           may trap us in a cycle (see is_essential()).
   *                           We further do not cache inverse values computed
   *                           while checking for essential checks.
   * @return True if there exists an inverse value for `x`.
   */
  virtual bool is_invertible(const VALUE& t,
                             uint64_t pos_x,
                             bool is_essential_check = false);

  /**
   * Check consistency condition for `x` at index `pos_x` with respect to
   * constant bits and target value `t`.
   * @param t The target value.
   * @param pos_x The index of `x`.
   * @return True if there exists a consistent value for `x`.
   */
  virtual bool is_consistent(const VALUE& t, uint64_t pos_x);

  /**
   * Get an inverse value for `x` at index `pos_x` with respect to constant bits
   * and target value `t`.
   * @param t The target value.
   * @param pos_x The index of `x`.
   * @return The inverse value for `x`.
   */
  virtual const VALUE& inverse_value(const VALUE& t, uint64_t pos_x);
  /**
   * Get an consistent value for `x` at index `pos_x` with respect to constant
   * bits and target value `t`.
   * @param t The target value.
   * @param pos_x The index of `x`.
   * @return The consistent value for `x`.
   */
  virtual const VALUE& consistent_value(const VALUE& t, uint64_t pos_x);

  /**
   * Select the next step in the propagation path based on target value `t` and
   * the current assignment of this node's children.
   * @param t The target value of this node.
   * @return The index of child to propagate the target value down to.
   */
  virtual uint64_t select_path(const VALUE& t);

  /**
   * Get child at given index.
   * @param pos The index of the child to get.
   * @return The child at the given index.
   */
  Node<VALUE>* operator[](uint64_t pos) const;

  /**
   * Get the arity of this node.
   * @return The arity of this node.
   */
  uint32_t arity() const { return d_arity; }
  /**
   * Set the assignment of this node.
   * @param assignment The assignment to set.
   */
  virtual void set_assignment(const VALUE& assignment) = 0;
  /**
   * Get the assignment of this node.
   * @return The assignment of this node.
   */
  const VALUE& assignment() const { return d_assignment; }

  /**
   * Set id of this node.
   * @param id The id to set.
   */
  void set_id(uint64_t id);
  /**
   * Set normalized id of this node.
   * @param id The id to set.
   */
  void set_normalized_id(uint64_t id);
  /**
   * Get id of this node.
   * @return The id of this node.
   */
  uint64_t id() const { return d_id; }
  /**
   * Get normalized id of this node.
   * @return The id of this node.
   */
  uint64_t normalized_id() const { return d_normalized_id; }

  /** Get the string representation of this node. */
  virtual std::string str() const = 0;

  /**
   * Get logging info of this node.
   * @return A vector of strings, representng lines of logging info about the
   *         contents of this node.
   */
  virtual std::vector<std::string> log() const;

 protected:
  Node(RNG* rng, const VALUE& assignment, bool is_value);
  Node(RNG* rng, const VALUE& assignment, Node<VALUE>* child0, bool is_value);
  Node(RNG* rng,
       const VALUE& assignment,
       Node<VALUE>* child0,
       Node<VALUE>* child1,
       bool is_value);
  Node(RNG* rng,
       const VALUE& assignment,
       Node<VALUE>* child0,
       Node<VALUE>* child1,
       Node<VALUE>* child2,
       bool is_value);

  /**
   * Helper to select a non-const operand. Additional collects the indices
   * of all non-const operands.
   * @note Asserts that at least one operand is non-const.
   * @param res_inputs The resulting vector of indices of non-const operands.
   * @return -1 if more than one operand is non-const (and thus
   *         `res_intpus.size() > 1`), else the index of the non-const operand.
   */
  virtual uint64_t select_path_non_const(
      std::vector<uint64_t>& res_inputs) const;

  /** The id of this node. */
  uint64_t d_id = 0;
  /**
   * The id of this node after normalization.
   *
   * It is guaranteed that ordering nodes (ascending) by this id corresponds to
   * their DAG post-order. This is relevant for cone updates, where we need
   * to update the assignments of children before we update their parents.
   * `d_id` originally has this property, but normalization may violate it
   * by "semi-destructive" (destructive, but can be reverted) rewriting
   * (see LocalSearchBV::normalize_extracts()). Thus, after normalization,
   * this id needs to be recomputed in a post-order DAG traversal manner.
   * If a LocalSearch implementation does not perform destructive rewriting, no
   * extra handling but setting this to the same value as `d_id` is required.
   */
  uint64_t d_normalized_id = 0;

  /** The children of this node. */
  std::vector<Node<VALUE>*> d_children;

  /** The associated random number generator. */
  RNG* d_rng;

  /** The current assignment of this node. */
  VALUE d_assignment;

  /** The arity of this node. */
  uint32_t d_arity;

  /**
   * True if this node is a value.
   * @note For bit-vector nodes, this indicates that the underlying domain
   *       is fixed.
   */
  bool d_is_value;
  /** True if all children of this node are values. */
  bool d_all_value;

  /** Cached inverse value result. */
  std::unique_ptr<VALUE> d_inverse;
  /** Cached consistent value result. */
  std::unique_ptr<VALUE> d_consistent;
};

std::ostream& operator<<(std::ostream& out, const Node<BitVector>& node);

}  // namespace ls
}  // namespace bzla
#endif