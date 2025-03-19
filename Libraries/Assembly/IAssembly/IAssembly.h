/*
 * IAssembly.h
 *
 *  Created on: Oct 20, 2017
 *      Author: cgarnier
 */

#ifndef ASSEMBLY_IASSEMBLY_IASSEMBLY_H_
#define ASSEMBLY_IASSEMBLY_IASSEMBLY_H_

class IAssembly
{
public:
    IAssembly() = default;
    virtual ~IAssembly() = default;

    virtual void start_Assembly() = 0;
};

#endif /* ASSEMBLY_IASSEMBLY_IASSEMBLY_H_ */
