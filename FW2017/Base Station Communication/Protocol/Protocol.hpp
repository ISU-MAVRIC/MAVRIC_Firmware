/*
 * Protocol.h
 *
 *  Created on: Oct 1, 2016
 *      Author: James
 */

#ifndef PROTOCOL_PROTOCOL_HPP_
#define PROTOCOL_PROTOCOL_HPP_

bool IsComplete(IncomingHeader header, int count);
Incoming Decode(unsigned char* data);

#endif /* PROTOCOL_PROTOCOL_HPP_ */
