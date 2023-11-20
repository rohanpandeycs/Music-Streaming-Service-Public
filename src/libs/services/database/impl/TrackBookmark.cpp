/*
 * Copyright (C) 2020 Emeric Poupon
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

#include "services/database/TrackBookmark.hpp"

#include "services/database/Session.hpp"
#include "services/database/Track.hpp"
#include "services/database/User.hpp"
#include "IdTypeTraits.hpp"
#include "Utils.hpp"

namespace Database {

TrackBookmark::TrackBookmark(ObjectPtr<User> user, ObjectPtr<Track> track)
: _user {getDboPtr(user)},
_track {getDboPtr(track)}
{
}

TrackBookmark::pointer
TrackBookmark::create(Session& session, ObjectPtr<User> user, ObjectPtr<Track> track)
{
	return session.getDboSession().add(std::unique_ptr<TrackBookmark> {new TrackBookmark {user, track}});
}

std::size_t
TrackBookmark::getCount(Session& session)
{
	session.checkSharedLocked();

	return session.getDboSession().query<int>("SELECT COUNT(*) FROM track_bookmark");
}

RangeResults<TrackBookmarkId>
TrackBookmark::find(Session& session, UserId userId, Range range)
{
	session.checkSharedLocked();

	auto query {session.getDboSession().query<TrackBookmarkId>("SELECT id from track_bookmark")
				.where("user_id = ?").bind(userId)};

	return Utils::execQuery(query, range);
}

TrackBookmark::pointer
TrackBookmark::find(Session& session, UserId userId, TrackId trackId)
{
	session.checkSharedLocked();

	return session.getDboSession().find<TrackBookmark>()
		.where("user_id = ?").bind(userId)
		.where("track_id = ?").bind(trackId)
		.resultValue();
}

TrackBookmark::pointer
TrackBookmark::find(Session& session, TrackBookmarkId id)
{
	session.checkSharedLocked();

	return session.getDboSession().find<TrackBookmark>()
		.where("id = ?").bind(id)
		.resultValue();
}

} // namespace Database

