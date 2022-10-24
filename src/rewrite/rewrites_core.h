#ifndef BZLA_REWRITE_REWRITES_BOOL_H_INCLUDED
#define BZLA_REWRITE_REWRITES_BOOL_H_INCLUDED

#include "rewrite/rewriter.h"

namespace bzla {

/* equal -------------------------------------------------------------------- */

template <>
Node RewriteRule<RewriteRuleKind::EQUAL_EVAL>::_apply(Rewriter& rewriter,
                                                      const Node& node);
template <>
Node RewriteRule<RewriteRuleKind::EQUAL_SPECIAL_CONST>::_apply(
    Rewriter& rewriter, const Node& node);
// true_eq
template <>
Node RewriteRule<RewriteRuleKind::EQUAL_TRUE>::_apply(Rewriter& rewriter,
                                                      const Node& node);
// false_eq
template <>
Node RewriteRule<RewriteRuleKind::EQUAL_FALSE>::_apply(Rewriter& rewriter,
                                                       const Node& node);
// bcond_eq
template <>
Node RewriteRule<RewriteRuleKind::EQUAL_ITE>::_apply(Rewriter& rewriter,
                                                     const Node& node);
// bcond_if_eq
template <>
Node RewriteRule<RewriteRuleKind::EQUAL_ITE_BV1>::_apply(Rewriter& rewriter,
                                                         const Node& node);
// bcond_uneq_if_eq
template <>
Node RewriteRule<RewriteRuleKind::EQUAL_ITE_DIS_BV1>::_apply(Rewriter& rewriter,
                                                             const Node& node);
// add_left_eq
// add_right_eq
template <>
Node RewriteRule<RewriteRuleKind::EQUAL_ADD>::_apply(Rewriter& rewriter,
                                                     const Node& node);
// add_add_1_eq
// add_add_2_eq
// add_add_3_eq
// add_add_4_eq
template <>
Node RewriteRule<RewriteRuleKind::EQUAL_ADD_ADD>::_apply(Rewriter& rewriter,
                                                         const Node& node);
// concat_eq
template <>
Node RewriteRule<RewriteRuleKind::EQUAL_CONCAT>::_apply(Rewriter& rewriter,
                                                        const Node& node);

/* distinct ----------------------------------------------------------------- */

template <>
Node RewriteRule<RewriteRuleKind::DISTINCT_CARD>::_apply(Rewriter& rewriter,
                                                         const Node& node);

/* --- Elimination Rules ---------------------------------------------------- */

template <>
Node RewriteRule<RewriteRuleKind::DISTINCT_ELIM>::_apply(Rewriter& rewriter,
                                                         const Node& node);

/* -------------------------------------------------------------------------- */
}  // namespace bzla
#endif