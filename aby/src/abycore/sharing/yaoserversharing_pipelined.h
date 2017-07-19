/**
 \file 		yaoserversharing_pipelined.h
 \author	michael.zohner@ec-spride.de
 \copyright	ABY - A Framework for Efficient Mixed-protocol Secure Two-party Computation
			Copyright (C) 2015 Engineering Cryptographic Protocols Group, TU Darmstadt
			This program is free software: you can redistribute it and/or modify
			it under the terms of the GNU Affero General Public License as published
			by the Free Software Foundation, either version 3 of the License, or
			(at your option) any later version.
			This program is distributed in the hope that it will be useful,
			but WITHOUT ANY WARRANTY; without even the implied warranty of
			MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
			GNU Affero General Public License for more details.
			You should have received a copy of the GNU Affero General Public License
			along with this program. If not, see <http://www.gnu.org/licenses/>.
 \brief		Yao Server Sharing Pipelined class.
 */

#ifndef __YAOSERVERPIPELINEDSHARING_H__
#define __YAOSERVERPIPELINEDSHARING_H__

#include "sharing.h"
#include <algorithm>
#include "yaosharing.h"



//#define DEBUGYAOPIPESERVER
/**
 Yao Server Sharing pipelined class.
 */
class YaoServerPipeSharing: public YaoSharing {

public:
	/**
	 Constructor of the class.
	 */
	YaoServerPipeSharing(e_role role, uint32_t sharebitlen, ABYCircuit* circuit, crypto* crypt) :
			YaoSharing(role, sharebitlen, circuit, crypt) {
		m_cBoolCircuit = new BooleanCircuit(m_pCircuit, m_eRole, S_YAO_PIPE);
		InitServer();
	}
	;
	/**
	 Destructor of the class.
	 */
	~YaoServerPipeSharing() {
		Reset();
	}
	;

	//MEMBER FUNCTIONS FROM SUPER CLASS YAO SHARING
	void Reset();
	void PrepareSetupPhase(ABYSetup* setup);
	void PerformSetupPhase(ABYSetup* setup);
	void FinishSetupPhase(ABYSetup* setup);
	void EvaluateLocalOperations(uint32_t level);
	void EvaluateInteractiveOperations(uint32_t level);
	void SendConversionValues(uint32_t gateid);

	void FinishCircuitLayer(uint32_t level);

	void PrepareOnlinePhase();

	void InstantiateGate(GATE* gate);
	void UsedGate(uint32_t gateid);

	void GetDataToSend(vector<BYTE*>& sendbuf, vector<uint32_t>& bytesize);
	void GetBuffersToReceive(vector<BYTE*>& rcvbuf, vector<uint32_t>& rcvbytes);

	uint32_t AssignInput(CBitVector& input);
	uint32_t GetOutput(CBitVector& out);

	const char* sharing_type() {
		return "Yao server pipelined";
	}
	;
	//ENDS HERE..

private:
	//Global constant key
	CBitVector m_vR; /**< _____________*/
	//Permutation bits for the servers input keys
	CBitVector m_vPermBits; /**< _____________*/
	//Random values from output of ot extension
	vector<CBitVector> m_vROTMasks; /**< Masks_______________*/
	uint32_t m_nClientInputKexIdx; /**< Client __________*/
	uint32_t m_nClientInputKeyCtr; /**< Client __________*/

	CBitVector m_vServerKeySndBuf; /**< Server Key Sender Buffer*/
	vector<CBitVector> m_vClientKeySndBuf; /**< Client Key Sender Buffer*/
	CBitVector m_vClientROTRcvBuf; /**< Client ______________*/

	//vector<CBitVector> m_vClientConversionKeySndBuf;
	//CBitVector			m_vClientCOnversionROTRcvBuf;

	CBitVector m_vOutputShareSndBuf; /**< Output Share Sender Buffer.*/
	CBitVector m_vOutputShareRcvBuf; /**< Output Share Receiver Buffer.*/

	vector<GATE*> m_vServerOutputGates; /**< Server Output Gates*/

	uint32_t m_nOutputShareRcvCtr; /**< Output Share Receiver Counter*/

	uint64_t m_nPermBitCtr; /**< _____________*/
	uint32_t m_nServerInBitCtr; /**< _____________*/

	uint32_t m_nServerKeyCtr; /**< _____________*/
	uint32_t m_nClientInBitCtr; /**< _____________*/

	uint8_t* m_bLMaskBuf[2]; /**< _____________*/
	uint8_t* m_bRMaskBuf[2]; /**< _____________*/
	uint8_t* m_bLKeyBuf; /**< _____________*/
	uint8_t* m_bOKeyBuf[2]; /**< _____________*/
	uint8_t* m_bTmpBuf;
	//CBitVector

	vector<uint32_t> m_vClientInputGate; /**< _____________*/
	deque<input_gate_val_t> m_vPreSetInputGates;/**< _____________*/
	deque<a2y_gate_pos_t> m_vPreSetA2YPositions;/**< _____________*/
	e_role* m_vOutputDestionations; /** <  _____________*/
	uint32_t m_nOutputDestionationsCtr;

	non_lin_on_layers* 	m_vANDsOnLayers;


	//deque<uint32_t> 			m_vClientInputGate;

	/**Initialising the server. */
	void InitServer();
	/**Initialising a new layer.*/
	void InitNewLayer();

	/**
	 Creating Random wire keys.
	 \param	vec 		Bit vector.
	 \param 	numkeys		number of keys.
	 */
	void CreateRandomWireKeys(CBitVector& vec, uint32_t numkeys);
	/**
	 Creating and sending Garbled Circuit.
	 \param 	setup 	ABYSetup Object.
	 */
	void CreateAndSendGarbledCircuit(ABYSetup* setup);
	/**
	 Receiving the garbled circuit object.
	 */
	void ReceiveGarbledCircuit();
	/**
	 Method for evaluating XOR gate for the inputted
	 gate object.
	 \param gate		Gate Object
	 */
	void EvaluateXORGate(GATE* gate);
	/**
	 Method for evaluating AND gate for the inputted
	 gate object.
	 \param gate		Gate Object
	 */
	void EvaluateANDGate(GATE* gate);
	/**
	 Method for evaluating SIMD gate for the inputted
	 gateid.
	 \param gateid		Gate identifier
	 */
	void EvaluateSIMDGate(uint32_t gateid);
	/**
	 Method for evaluating Inversion gate for the inputted
	 gate object.
	 \param gate		Gate Object
	 */
	void EvaluateInversionGate(GATE* gate);
	/**
	 Method for evaluating conversion gate for the inputted
	 gateid.
	 \param gateid		Gate Identifier
	 */
	void EvaluateConversionGate(uint32_t gateid);
	/**
	 Method for creating garbled table.
	 \param ggate	gate Object.
	 \param pos 		Position of the object in the queue.
	 \param gleft	left gate in the queue.
	 \param gright	right gate in the queue.
	 */
	void CreateGarbledTable(GATE* ggate, uint32_t pos, GATE* gleft, GATE* gright);
	/**
	 PrecomputeGC______________
	 \param queue 	Dequeue Object.
	 */
	void PrecomputeGC(deque<uint32_t>& queue);

	//void EvaluateClientOutputGate(GATE* gate);
	//void CollectClientOutputShares();
	/**
	 Method for evaluating Client Output gates for the inputted
	 gate object.
	 \param gate		Gate Object
	 */
	void EvaluateClientOutputGate(GATE* gate);
	/**
	 Method for evaluating Server Output gates for the inputted
	 gate object.
	 \param gate		Gate Object
	 */
	void EvaluateServerOutputGate(GATE* gate);
	/**
	 Send Server Key from the given gateid.
	 \param	gateid 	Gate Identifier
	 */
	void SendServerInputKey(uint32_t gateid);
	/**
	 Evaluate Server Gate from the given gateid.
	 \param	gateid 	Gate Identifier
	 */
	void EvaluateServerInputGate(uint32_t gateid);
	/**
	 Evaluate Client Gate from the given gateid.
	 \param	gateid 	Gate Identifier
	 */
	void EvaluateClientInputGate(uint32_t gateid);

	/**
	 Method for assigning Output shares.
	 */
	void AssignOutputShares();
};

#endif /* __YAOSERVERPIPELINEDSHARING_H__ */
