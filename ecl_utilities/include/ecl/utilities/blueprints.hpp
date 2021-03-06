/**
 * @file /include/ecl/utilities/blueprints.hpp
 *
 * @brief Parent templates for blueprints and blueprint factories.
 *
 * @date May 2009
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef ECL_UTILITIES_BLUEPRINT_FACTORY_HPP_
#define ECL_UTILITIES_BLUEPRINT_FACTORY_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/concepts/blueprints.hpp>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace ecl {

/**
 * @brief This is a parent template for blueprints using crtp.
 *
 * Blueprints of various sorts are often passed to a constructor of their
 * associated class. If a template is used to in the constructor call, e.g.
 *
 * @code
 * template<typename BluePrint>
 * A(const BluePrint& blueprint);
 * @endcode
 *
 * then this tends to create function collisions with other template
 * functions (especially streaming operators). To avoid this, we need
 * a parent class, so for this we use the crtp style (because it avoids
 * any virtual costs).
 *
 * Classes that are of type Derived must satisfy the blueprint concept
 * (ecl::concepts::BluePrint).
 */
template <typename Derived>
class BluePrint {
public:
	/**
	 * @brief Blueprint concept checking.
	 *
	 * Check that the derived class satisfies the blueprint concept.
	 */
	BluePrint() {
        ecl_compile_time_concept_check(BluePrintConcept<Derived>);
	}
	/**
	 * @brief Use the blueprint to instantiate a new object.
	 *
	 * A crtp virtual call to its children's instantiate method. The template
	 * argument is used, because the crtp base cannot catch typedef's in its
	 * children at this point of the compilation. With the template parameter
	 * this check is delayed until enough of this base class is known.
	 *
	 * Note, this cannot have the same name as the blueprint method because
	 * then the child will inherit it, even if it doesn't have the class and
	 * the concept check will fail.
	 *
	 * @return BaseType : the type to be generated by the blueprint.
	 */
	template <typename BaseType>
	BaseType implementInstantiate() {
		return static_cast<Derived*>(this)->instantiate();
	}
	/**
	 * @brief Apply the blueprint rules to an existing object.
	 *
	 * A crtp virtual call to its children's apply method. This acts on the
	 * blueprint's target class to configure it according to the blueprint.
	 * The template parameter is a trick to delay the determination of the
	 * target type, because at this point of the compilation it cannot
	 * retrieve the base_type from the derived type.
	 *
	 * Note, this cannot have the same name as the blueprint method because
	 * then the child will inherit it, even if it doesn't have the class and
	 * the concept check will fail.
	 *
	 * @param object : the object to be configured.
	 */
	template <typename BaseType>
	void implementApply(BaseType& object) const {
		static_cast<const Derived*>(this)->apply(object);
	}

	virtual ~BluePrint() {}
};
/**
 * @brief Primary template for blueprint factories.
 *
 * This is often used as a conveniently inheritable class for mixing in
 * static blueprint methods within the class that is being generated.
 * Mostly for use internally within the ecl.
 */
template <typename Base>
class BluePrintFactory {
    private:
        /**
         * @brief Default constructor that locks this class from being used directly.
         */
        BluePrintFactory() {};
        virtual ~BluePrintFactory() {};
};

}; // namespace ecl


#endif /* ECL_UTILITIES_BLUEPRINT_FACTORY_HPP_ */
