/***********************************************************************************************************************
*                                                                                                                      *
* scopehal-plugins-gpl                                                                                                 *
*                                                                                                                      *
* Copyright (c) 2012-2025 Andrew D. Zonenberg and contributors                                                         *
* All rights reserved.                                                                                                 *
*                                                                                                                      *
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the     *
* following conditions are met:                                                                                        *
*                                                                                                                      *
*    * Redistributions of source code must retain the above copyright notice, this list of conditions, and the         *
*      following disclaimer.                                                                                           *
*                                                                                                                      *
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the       *
*      following disclaimer in the documentation and/or other materials provided with the distribution.                *
*                                                                                                                      *
*    * Neither the name of the author nor the names of any contributors may be used to endorse or promote products     *
*      derived from this software without specific prior written permission.                                           *
*                                                                                                                      *
* THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   *
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL *
* THE AUTHORS BE HELD LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES        *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR       *
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                                                                          *
*                                                                                                                      *
***********************************************************************************************************************/
/**
	@file
	@author Mike Walters
	@brief Declaration of SCPILinuxGPIBTransport
 */

#ifdef HAS_LINUXGPIB

#ifndef SCPILinuxGPIBTransport_h
#define SCPILinuxGPIBTransport_h

/**
	@brief Abstraction of a transport layer for moving SCPI data between endpoints
 */
class SCPILinuxGPIBTransport : public SCPITransport
{
public:
	SCPILinuxGPIBTransport(const std::string& args);
	~SCPILinuxGPIBTransport();

	//not copyable or assignable
	SCPILinuxGPIBTransport(const SCPILinuxGPIBTransport&) =delete;
	SCPILinuxGPIBTransport& operator=(const SCPILinuxGPIBTransport&) =delete;

	std::string GetConnectionString() override;
	static std::string GetTransportName();

	void FlushRXBuffer(void) override;
	bool SendCommand(const std::string& cmd) override;
	std::string ReadReply(bool endOnSemicolon = true, std::function<void(float)> progress = nullptr) override;
	size_t ReadRawData(size_t len, unsigned char* buf, std::function<void(float)> progress = nullptr) override;
	void SendRawData(size_t len, const unsigned char* buf) override;

	bool IsCommandBatchingSupported() override;
	bool IsConnected() override;

	TRANSPORT_INITPROC(SCPILinuxGPIBTransport)

protected:
	std::string m_devicePath;

	int m_handle = -1;
	int m_board_index;
	int m_pad;
	int m_sad = 0;
	int m_timeout = 10;
};

#endif

#endif
