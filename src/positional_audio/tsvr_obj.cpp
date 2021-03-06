#include "tsvr_obj.h"

#include "guildwarstwo.h"

// Getters

auto TsVrObj::get_vr() const -> std::wstring
{
    return m_vr;
}

auto TsVrObj::get_vr_description() const -> std::wstring
{
    return m_vr_desc;
}

auto TsVrObj::get_context() const -> std::vector<std::byte>
{
    return m_context;
}

auto TsVrObj::get_context_as_string() const -> std::string
{
    return std::string{reinterpret_cast<const char *>(m_context.data()), m_context.size()};
}

auto TsVrObj::getIdentity() const -> std::wstring
{
    if (m_CustomEnvironmentSupport)
    {
        CustomEnvironmentSupportInterface *iCustomEnvironmentSupport =
        qobject_cast<CustomEnvironmentSupportInterface *>(m_CustomEnvironmentSupport);
        return iCustomEnvironmentSupport->get_identity();
    }
    else
        return m_identityRaw;
}

auto TsVrObj::getIdentityRaw() const -> std::wstring
{
    return m_identityRaw;
}

auto TsVrObj::get_avatar() const -> thorwe::Three_Dee_Info
{
    return m_avatar;
}


//! Handles the ts_infodata_qt event for this client
auto TsVrObj::onInfoDataChanged(QTextStream &data) -> bool
{
    bool isDirty = false;
    if (!m_vr.empty())
    {
        data << "Positional Audio: ";
        isDirty = true;

        data << "\nPlaying " << QString::fromStdWString(get_vr());
        const auto ident = getIdentity();
        if (!ident.empty())
        {
            data << " as " << QString::fromStdWString(ident);

            if (m_CustomEnvironmentSupport)
            {
                CustomEnvironmentSupportInterface *iCustomEnvironmentSupport =
                qobject_cast<CustomEnvironmentSupportInterface *>(m_CustomEnvironmentSupport);
                isDirty |= iCustomEnvironmentSupport->onInfoData(data);
            }
        }
    }
    return isDirty;
}

// Slot Setters

void TsVrObj::setCustomEnvironmentSupport(QObject *val)
{
    m_CustomEnvironmentSupport = val;
}

void TsVrObj::set_vr(std::wstring_view val)
{
    if (m_vr == val)
        return;

    m_vr = val;

    if (m_CustomEnvironmentSupport)
        m_CustomEnvironmentSupport->deleteLater();

    if (m_vr.empty())
        m_CustomEnvironmentSupport = nullptr;
    else if (m_vr == L"Guild Wars 2")
        m_CustomEnvironmentSupport = new GuildWarsTwo(this);

}

void TsVrObj::set_vr_description(std::wstring_view val)
{
    if (m_vr_desc == val)
        return;

    m_vr_desc = val;
}

void TsVrObj::set_context(const std::vector<std::byte> &val)
{
    if (m_context == val)
        return;

    m_context = val;
}

void TsVrObj::setIdentityRaw(std::wstring_view val)
{
    if (m_identityRaw == val)
        return;

    m_identityRaw = val;

    if (m_CustomEnvironmentSupport)
    {
        CustomEnvironmentSupportInterface *iCustomEnvironmentSupport =
        qobject_cast<CustomEnvironmentSupportInterface *>(m_CustomEnvironmentSupport);
        iCustomEnvironmentSupport->on_identity_raw_dirty(m_identityRaw);
    }
}

void TsVrObj::reset_avatar()
{
    m_avatar.position = {0.f, 0.f, 0.f};
    m_avatar.front = {0.f, 0.f, 0.f};
    m_avatar.top = {0.f, 0.f, 0.f};
}

bool operator==(const TS3_VECTOR &veca, const TS3_VECTOR &vecb)
{
    return ((veca.x == vecb.x) && (veca.y == vecb.y) && (veca.z == vecb.z));
}
