/*
 * Copyright (C) 2019 Emeric Poupon
 *
 * This file is part of LMS.
 *
 * LMS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LMS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LMS.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <optional>
#include <string>

#include "services/database/UserId.hpp"

namespace Database
{
	class Db;
	class Session;
}

namespace Wt
{
	class WEnvironment;
}

namespace Wt::Http
{
	class Request;
}

namespace Auth
{
	class IEnvService
	{
		public:
			virtual ~IEnvService() = default;

			// Auth Token services
			struct CheckResult
			{
				enum class State
				{
					Granted,
					Denied,
					Throttled,
				};

				State state {State::Denied};
				std::optional<Database::UserId>	userId {};
			};

			virtual CheckResult			processEnv(const Wt::WEnvironment& env) = 0;
			virtual CheckResult			processRequest(const Wt::Http::Request& request) = 0;
	};

	std::unique_ptr<IEnvService> createEnvService(std::string_view backendName, Database::Db& db);
} // namespace Auth
