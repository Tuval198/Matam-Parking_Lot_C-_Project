#ifndef EX3_UNIQUEARRAYIMP_H
#define EX3_UNIQUEARRAYIMP_H


template <class Element, class Compare>
UniqueArray<Element, Compare>::UniqueArray(unsigned int size) :
    array(new Element*[size]), size(size) {
    for(int i = 0; i < size ; i++){
        array[i] = NULL;
    }
}

template <class Element, class Compare>
UniqueArray<Element,Compare>::UniqueArray(const UniqueArray &other):
    array(new Element*[other.size]) , size(other.size){
    for(int i = 0; i < size ; i++){
        if(other.array[i] == NULL){
            array[i] = NULL;
        } else {
            //use of copy constructor for Element:
            //will have to be deleted from memory in remove() function
            array[i] = new Element(*(other.array[i]));
        }
    }
}

template <class Element, class Compare>
UniqueArray<Element,Compare>::~UniqueArray() {
    for(int i = 0; i < size; i++){
        if(array[i] != NULL){
            remove(*array[i]);
        }
    }
    delete[] array;
}

template <class Element, class Compare>
bool UniqueArray<Element,Compare>::remove(const Element &element) {
    Compare compare_function;
    for(int i = 0; i < size; i++){
       if(array[i]!=NULL && compare_function(*array[i], element)){
            //element found
            delete array[i];
            array[i] = NULL;
            return true;
        }
    }
    //if we are here element was not found
    return false;
}

template <class Element, class Compare>
unsigned int UniqueArray<Element,Compare>::insert(const Element &element) {
    //check if already in the set:
    unsigned int index;
    if(getIndex(element,index)){
        return index;
    }
    //will insert the element to the first slot which is not NULL
    for(int i = 0; i < size; i++){
        if(array[i] == NULL){
            //found empty slot
            array[i] =  new Element(element);
            return i;
        }
    }
    //if we are here - there is no empty space in the array
    throw UniqueArrayIsFullException();
}

template <class Element, class Compare>
bool UniqueArray<Element,Compare>::getIndex(const Element &element,
                                                    unsigned int &index) const {
    Compare compare_function;
    for(int i = 0; i < size; i++){
        if(array[i] != NULL && compare_function(*(array[i]),element)){
            index = i;
            return true;
        }
    }
    //if we are here element was not found
    return false;
}

template <class Element, class Compare>
const Element* UniqueArray<Element,Compare>::operator[](
        const Element &element) const {
    unsigned int index;
    if(!getIndex(element, index)){
        return NULL; //element not in unique array
    }
    return array[index];
}

template <class Element, class Compare>
unsigned int UniqueArray<Element,Compare>::getCount() const {
    unsigned int count = 0;
    for(int i = 0; i < size; i++){
        if(array[i] != NULL){
            count ++;
        }
    }
    return count;
}

template <class Element, class Compare>
unsigned int UniqueArray<Element,Compare>::getSize() const {
    return size;
}

template <class Element, class Compare>
UniqueArray<Element,Compare> UniqueArray<Element,Compare>::filter(
        const UniqueArray<Element, Compare>::Filter &f) const {
    UniqueArray new_unique_array(size);
    for(int i =0; i < size; i++){
        if(array[i]!=NULL && f(*array[i])){
            new_unique_array.array[i] = new Element(*array[i]);
        }
    }
    return new_unique_array;
}

template  <class Element, class Compare>
Element* UniqueArray<Element,Compare>::getByIndex(const unsigned int index) {
    if(index >= size){
        return NULL; //out of bound
    }
    return array[index];
}

template  <class Element, class Compare>
const Element* UniqueArray<Element,Compare>::
        getByIndex(const unsigned int index) const {
    if(index >= size){
        return NULL; //out of bound
    }
    return array[index];
}



#endif //EX3_UNIQUEARRAYIMP_H
