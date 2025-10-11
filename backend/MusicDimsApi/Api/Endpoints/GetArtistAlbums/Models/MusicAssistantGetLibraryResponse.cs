using System.Text.Json.Serialization;

namespace MusicDimsApi.Endpoints.GetArtistAlbums.Models;

public record MusicAssistantLibraryResponse(
    [property: JsonPropertyName("changed_states")]
    IReadOnlyList<object> ChangedStates,
    [property: JsonPropertyName("service_response")]
    MusicAssistantLibraryResponse.ServiceResponseDto ServiceResponse)
{
    public record ServiceResponseDto(
        [property: JsonPropertyName("artists")]
        IReadOnlyList<Artist> Artists,
        [property: JsonPropertyName("albums")]
        IReadOnlyList<Album> Albums,
        [property: JsonPropertyName("tracks")]
        IReadOnlyList<Track> Tracks,
        [property: JsonPropertyName("playlists")]
        IReadOnlyList<Playlist> Playlists,
        [property: JsonPropertyName("radio")]
        IReadOnlyList<RadioDto> Radio,
        [property: JsonPropertyName("audiobooks")]
        IReadOnlyList<Audiobook> Audiobooks,
        [property: JsonPropertyName("podcasts")]
        IReadOnlyList<Podcast> Podcasts);

    public record Artist(
        [property: JsonPropertyName("media_type")]
        string MediaType,
        [property: JsonPropertyName("uri")]
        string Uri,
        [property: JsonPropertyName("name")]
        string Name,
        [property: JsonPropertyName("version")]
        string Version,
        [property: JsonPropertyName("image")]
        string? Image);

    public record Album(
        [property: JsonPropertyName("media_type")]
        string MediaType,
        [property: JsonPropertyName("uri")]
        string Uri,
        [property: JsonPropertyName("name")]
        string Name,
        [property: JsonPropertyName("version")]
        string Version,
        [property: JsonPropertyName("image")]
        string? Image,
        [property: JsonPropertyName("artists")]
        IReadOnlyList<Artist> Artists);

    public record Track(
        [property: JsonPropertyName("media_type")]
        string MediaType,
        [property: JsonPropertyName("uri")]
        string Uri,
        [property: JsonPropertyName("name")]
        string Name,
        [property: JsonPropertyName("version")]
        string Version,
        [property: JsonPropertyName("image")]
        string? Image);

    public record Playlist(
        [property: JsonPropertyName("media_type")]
        string MediaType,
        [property: JsonPropertyName("uri")]
        string Uri,
        [property: JsonPropertyName("name")]
        string Name,
        [property: JsonPropertyName("version")]
        string Version,
        [property: JsonPropertyName("image")]
        string? Image);

    public record RadioDto(
        [property: JsonPropertyName("media_type")]
        string MediaType,
        [property: JsonPropertyName("uri")]
        string Uri,
        [property: JsonPropertyName("name")]
        string Name,
        [property: JsonPropertyName("version")]
        string Version,
        [property: JsonPropertyName("image")]
        string? Image);

    public record Audiobook(
        [property: JsonPropertyName("media_type")]
        string MediaType,
        [property: JsonPropertyName("uri")]
        string Uri,
        [property: JsonPropertyName("name")]
        string Name,
        [property: JsonPropertyName("version")]
        string Version,
        [property: JsonPropertyName("image")]
        string? Image);

    public record Podcast(
        [property: JsonPropertyName("media_type")]
        string MediaType,
        [property: JsonPropertyName("uri")]
        string Uri,
        [property: JsonPropertyName("name")]
        string Name,
        [property: JsonPropertyName("version")]
        string Version,
        [property: JsonPropertyName("image")]
        string? Image);
}