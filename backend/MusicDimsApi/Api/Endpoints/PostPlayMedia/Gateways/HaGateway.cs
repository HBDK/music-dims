using System.Text.Json;
using System.Text.Json.Serialization;
using MusicDimsApi.Clients;

namespace MusicDimsApi.Endpoints.PostPlayMedia.Gateways;

public class HaGateway(IHttpClientFactory clientFactory, ILogger<HaGateway> logger) : IHaGateway
{
    private const string PostPlayMediaPath = "/api/services/music_assistant/play_media"; //"?return_response=true";

    public async Task PlayMedia(string playerId, string mediaId, string mediaType)
    {
        var body = new PostPlayMediaBody(
            $"media_player.{playerId}",
            mediaType,
            mediaId,
            "replace");

        var client = clientFactory.CreateClient(HomeAssistantClient.Name);

        var response = await client.PostAsync(
            PostPlayMediaPath,
            new StringContent(
                JsonSerializer.Serialize(body),
                System.Text.Encoding.UTF8,
                "application/json"));

        var test = await response.Content.ReadAsStringAsync();
        response.EnsureSuccessStatusCode();
    }
}

public record PostPlayMediaBody(
    [property: JsonPropertyName("entity_id")]
    string EntityId,
    [property: JsonPropertyName("media_type")]
    string MediaType,
    [property: JsonPropertyName("media_id")]
    string MediaId,
    [property: JsonPropertyName("enqueue")]
    string Enqueue);

public interface IHaGateway
{
    Task PlayMedia(string playerId, string mediaId, string mediaType);
}