#include <algorithm>
#include <optional>
#include <span>

#include "range/v3/range/concepts.hpp"
#include "range/v3/view/all.hpp"
#include "range/v3/view/split.hpp"
#include "range/v3/algorithm/search.hpp"

template <ranges::contiguous_range V, ranges::forward_range Pattern>
requires ranges::view_<V> && ranges::view_<Pattern> &&
    std::indirectly_comparable<ranges::iterator_t<V>,
                               ranges::iterator_t<Pattern>, ranges::equal_to>
class contig_split_view
    : public ranges::view_interface<contig_split_view<V, Pattern>> {
  public:
    contig_split_view() = default;
    contig_split_view(V base, Pattern pattern)
        : base_(base), pattern_(pattern) {}

    template <ranges::contiguous_range R>
    requires std::constructible_from<V, ranges::views::all_t<R>> &&
        std::constructible_from<Pattern,
                                ranges::single_view<ranges::range_value_t<R>>>
        contig_split_view(R&& r, ranges::range_value_t<R> elem)
        : base_(ranges::views::all(std::forward<R>(r))),
          pattern_(std::move(elem)) {}

    struct sentinel;
    struct as_sentinel_t {};

    class iterator {
      private:
        using underlying =
            std::remove_reference_t<ranges::range_reference_t<V>>;
        friend class sentinel;

        contig_split_view* parent = nullptr;
        ranges::iterator_t<V> cur = ranges::iterator_t<V>();
        ranges::iterator_t<V> next = ranges::iterator_t<V>();

      public:
        iterator() = default;
        iterator(contig_split_view* p)
            : parent(p),
              cur(std::ranges::begin(p->base_)),
              next(lookup_next()) {}

        iterator(as_sentinel_t, contig_split_view* p)
            : parent(p), cur(std::ranges::end(p->base_)), next() {}

        using iterator_category = std::forward_iterator_tag;

        struct reference : std::span<underlying> {
            using std::span<underlying>::span;

            operator std::string_view()
                    const requires std::same_as < ranges::range_value_t<V>,
            char > { return {this->data(), this->size()}; }
        };

        using value_type = reference;
        using difference_type = std::ptrdiff_t;

        bool operator==(iterator const& rhs) const { return cur == rhs.cur; }

        auto lookup_next() const -> ranges::iterator_t<V> {
            return ranges::search(
                       ranges::subrange(cur, ranges::end(parent->base_)),
                       parent->pattern_)
                .begin();
        }

        auto operator++() -> iterator& {
            cur = next;
            if (cur != ranges::end(parent->base_)) {
                cur += ranges::distance(parent->pattern_);
                next = lookup_next();
            }
            return *this;
        }
        auto operator++(int) -> iterator {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        auto operator*() const -> reference { return {cur, next}; }
    };

    struct sentinel {
        bool operator==(iterator const& rhs) const {
            return rhs.cur == sentinel;
        }

        ranges::sentinel_t<V> sentinel;
    };

    auto begin() -> iterator {
        if (not cached_begin_) {
            cached_begin_.emplace(this);
        }
        return *cached_begin_;
    }
    auto end() -> sentinel { return {ranges::end(base_)}; }

    auto end() -> iterator requires ranges::common_range<V> {
        return {as_sentinel_t(), this};
    }

  private : V base_ = V();
    Pattern pattern_ = Pattern();
    std::optional<iterator> cached_begin_;
};

namespace ranges {
    template<contiguous_range V, forward_range Pattern>
    requires view_<V> && view_<Pattern>
      && indirectly_comparable<
        iterator_t<V>, iterator_t<Pattern>, equal_to>
    class split_view<V, Pattern>
        : public contig_split_view<V, Pattern>
    {
        using base = contig_split_view<V, Pattern>;
        using base::base;
    };
}




struct zstring_sentinel {
    bool operator==(char const* p) const { return *p == '\0'; }
};

struct zstring : ranges::view_interface<zstring> {
    char const* p = nullptr;
    zstring() = default;
    zstring(char const* p) : p(p) {}
    auto begin() const { return p; }
    auto end() const { return zstring_sentinel{}; }
};
