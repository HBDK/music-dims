using System.Text.Json.Serialization;

namespace MusicDimsApi.Endpoints.GetPlayer.Models;

public record GetPlayerStateResponseDto
{
    [JsonPropertyName("state")]
    public required string State { get; init; }

    [JsonPropertyName("volume")]
    public required int Volume { get; init; }

    [JsonPropertyName("muted")]
    public bool Muted { get; init; }

    [JsonPropertyName("title")]
    public string? Title { get; init; }

    [JsonPropertyName("artist")]
    public string? Artist { get; init; }

    [JsonPropertyName("album")]
    public string? Album { get; init; }

    [JsonPropertyName("picture-url")]
    public string? PictureUrl { get; init; }
}
