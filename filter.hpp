#ifndef FILTER__H__
#define FILTER__H__


namespace iter {

    //Forward declarations of Filter and filter
    template <typename FilterFunc, typename Container>
    class Filter;

    template <typename FilterFunc, typename Container>
    Filter<FilterFunc, Container> filter(FilterFunc, Container &);

    template <typename FilterFunc, typename Container>
    class Filter {
        // The filter function is the only thing allowed to create a Filter
        friend Filter filter<FilterFunc, Container>(FilterFunc, Container &);

        protected:
            // Type of the Container::Iterator, but since the name of that 
            // iterator can be anything, we have to grab it with this
            using contained_iter_type =
                decltype(((Container *)nullptr)->begin());

            // The type returned when dereferencing the Container::Iterator
            using contained_iter_ret =
                decltype(((contained_iter_type *)nullptr)->operator*());

            Container & container;
            FilterFunc filter_func;
            
            // Value constructor for use only in the filter function
            Filter(FilterFunc filter_func, Container & container) :
                container(container),
                filter_func(filter_func)
            { }
            Filter () = delete;
            Filter & operator=(const Filter &) = delete;
            // Default copy constructor used

        public:
            //  Holds an iterator of the contained type and a size_t for the
            //  index.  Each call to ++ increments both of these data members.
            //  Each dereference returns an IterYield.
            class Iterator {
                private:
                    contained_iter_type sub_iter;
                    const contained_iter_type sub_end;
                    FilterFunc filter_func;

                    // increment until the iterator points to is true on the 
                    // predicate.  Called by constructor and operator++
                    void skip_failures() { 
                        while (this->sub_iter != this->sub_end
                                && !this->filter_func(*this->sub_iter)) {
                            ++this->sub_iter;
                        }
                    }

                public:
                    Iterator (contained_iter_type iter,
                            contained_iter_type end,
                            FilterFunc filter_func) :
                        sub_iter(iter),
                        sub_end(end),
                        filter_func(filter_func)
                    { 
                        this->skip_failures();
                    } 

                    contained_iter_ret operator*() const {
                        return *this->sub_iter;
                    }

                    Iterator & operator++() { 
                        ++this->sub_iter;
                        this->skip_failures();
                        return *this;
                    }

                    bool operator!=(const Iterator & other) const {
                        return this->sub_iter != other.sub_iter;
                    }
            };

            Iterator begin() const {
                return Iterator(
                        this->container.begin(),
                        this->container.end(),
                        this->filter_func);
            }

            Iterator end() const {
                return Iterator(
                        this->container.end(),
                        this->container.end(),
                        this->filter_func);
            }

    };

    // Helper function to instantiate a Filter
    template <typename FilterFunc, typename Container>
    Filter<FilterFunc, Container> filter(
            FilterFunc filter_func, Container & container) {
        return Filter<FilterFunc, Container>(filter_func, container);
    }

}

#endif //ifndef FILTER__H__
