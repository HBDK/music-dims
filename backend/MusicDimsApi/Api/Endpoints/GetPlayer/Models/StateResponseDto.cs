using System.Text.Json.Serialization;

namespace MusicDimsApi.Endpoints.GetPlayer.Models;

public record HaPlayerStateResponse
{
    [JsonPropertyName("entity_id")]
    public string EntityId { get; init; } = string.Empty;

    [JsonPropertyName("state")]
    public string State { get; init; } = string.Empty;

    [JsonPropertyName("attributes")]
    public Attributes? AttributesInfo { get; init; }

    [JsonPropertyName("last_changed")]
    public DateTimeOffset? LastChanged { get; init; }

    [JsonPropertyName("last_reported")]
    public DateTimeOffset? LastReported { get; init; }

    [JsonPropertyName("last_updated")]
    public DateTimeOffset? LastUpdated { get; init; }

    [JsonPropertyName("context")]
    public Context? ContextInfo { get; init; }

    public record Attributes
    {
        [JsonPropertyName("volume_level")]
        public double? VolumeLevel { get; init; }

        [JsonPropertyName("is_volume_muted")]
        public bool? IsVolumeMuted { get; init; }

        [JsonPropertyName("media_content_id")]
        public string? MediaContentId { get; init; }

        [JsonPropertyName("media_content_type")]
        public string? MediaContentType { get; init; }

        [JsonPropertyName("media_duration")]
        public double? MediaDuration { get; init; }

        [JsonPropertyName("media_position")]
        public double? MediaPosition { get; init; }

        [JsonPropertyName("media_position_updated_at")]
        public DateTimeOffset? MediaPositionUpdatedAt { get; init; }

        [JsonPropertyName("media_title")]
        public string? MediaTitle { get; init; }

        [JsonPropertyName("media_artist")]
        public string? MediaArtist { get; init; }

        [JsonPropertyName("media_album_name")]
        public string? MediaAlbumName { get; init; }

        [JsonPropertyName("media_album_artist")]
        public string? MediaAlbumArtist { get; init; }

        [JsonPropertyName("app_id")]
        public string? AppId { get; init; }

        [JsonPropertyName("shuffle")]
        public bool? Shuffle { get; init; }

        [JsonPropertyName("repeat")]
        public string? Repeat { get; init; }

        [JsonPropertyName("mass_player_type")]
        public string? MassPlayerType { get; init; }

        [JsonPropertyName("active_queue")]
        public string? ActiveQueue { get; init; }

        [JsonPropertyName("device_class")]
        public string? DeviceClass { get; init; }

        [JsonPropertyName("entity_picture")]
        public string? EntityPicture { get; init; }

        [JsonPropertyName("icon")]
        public string? Icon { get; init; }

        [JsonPropertyName("friendly_name")]
        public string? FriendlyName { get; init; }

        [JsonPropertyName("supported_features")]
        public long? SupportedFeatures { get; init; }
    }

    public record Context
    {
        [JsonPropertyName("id")]
        public string? Id { get; init; }

        [JsonPropertyName("parent_id")]
        public string? ParentId { get; init; }

        [JsonPropertyName("user_id")]
        public string? UserId { get; init; }
    }
}
